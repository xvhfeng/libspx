/*************************************************************
 *                     _ooOoo_
 *                    o8888888o
 *                    88" . "88
 *                    (| -_- |)
 *                    O\  =  /O
 *                 ____/`---'\____
 *               .'  \\|     |//  `.
 *              /  \\|||  :  |||//  \
 *             /  _||||| -:- |||||-  \
 *             |   | \\\  -  /// |   |
 *             | \_|  ''\---/''  |   |
 *             \  .-\__  `-`  ___/-. /
 *           ___`. .'  /--.--\  `. . __
 *        ."" '<  `.___\_<|>_/___.'  >'"".
 *       | | :  `- \`.;`\ _ /`;.`/ - ` : | |
 *       \  \ `-.   \_ __\ /__ _/   .-` /  /
 *  ======`-.____`-.___\_____/___.-`____.-'======
 *                     `=---='
 *  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 *           佛祖保佑       永无BUG
 *
 * ==========================================================================
 *
 * this software or lib may be copied only under the terms of the gnu general
 * public license v3, which may be found in the source kit.
 *
 *       Filename:  spx_smart.c
 *        Created:  2018年03月16日 22时57分44秒
 *         Author:  Seapeak.Xu (www.94geek.com), xvhfeng@gmail.com
 *        Company:  Tencent Literature
 *         Remark:
 *
 ***********************************************************************/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "spx_allocator.h"
#include "spx_ctype.h"
#include "spx_mfunc.h"
#include "spx_ret.h"

/*
 * funtion wrap for alloc smart pointer
 * paras:
 *  kind : the smart pointer style
 *  size : the smart pointer memory size
 *  dtor : the destructor callback for smart pointer member
 *  err : errno
 * result:
 *  return the memory pointer as smart pointer
 */
__SpxPublic__ __SpxSmartptrType__(void *)
    spx_allocator_smartptr_new(spx_allocator_t *allocator,
                               const int sizeB,
                               spx_smartptr_dtor_fn *dtor,
                               spx_ret_t *ret) { /*{{{*/
    const int hlen = sizeof(spx_entry_smartptr_t);
    i32_t blen = spx_sys_align(sizeB);
    spx_errno_clear;
    spx_entry_smartptr_t *entry = (spx_entry_smartptr_t *) calloc(hlen + blen, sizeof(char));
    *ret = (spx_ret_t) errno;
    spx_log_exp_rtn(log, SpxLogError, !p, NULL, *ret,
                    "new smartptr.");
    spx_entry_base_set(entry, hlen + blen, sizeB, SpxEntrySmartptr);
    entry->ref_count = 1;
    entry->dtor = dtor;
    spx_klist_add_tail(&(entry->chain), &(allocator->smartptrs));
    spx_atomic_incr(allocator->smartptrs_count);
    spx_atomic_add(allocator->smartptrs_usesize, hlen + blen);
    return entry->buf;
} /*}}}*/

/*
 * cleanup callback for smart pointer
 * it's called auto by the variable out of the scope
 * paras:
 *  p : the smart pointer
 *
 */
__SpxPublic__ void
spx_allocator_smartptr_cleanup(void **p) { /*{{{*/
    union {
        void **rptr;
        void *ptr;
    } c = { .ptr = p };

    spx_entry_smartptr_t *entry = spx_container_of(*(c.rptr), spx_entry_smartptr_t, buf);
    spx_allocator_t *allocator = entry->allocator;
    if (SpxEntrySmartptr == entry->flag
        && (0 == entry->ref_count--)) {
        if (NULL != entry->dtor) {
            entry->dtor(c.rptr);
        }
        spx_kilst_del(&(entry->chain));
        spx_atomic_decr(allocator->smartptrs_count);
        spx_atomic_sub(allocator->smartptrs_usesize, entry->len);
        free(entry);
        *(c.rptr) = NULL;
        (c.rptr) = NULL;
    }
} /*}}}*/

/*
 *  incr sharedptr ref-count
 *  paras:
 *      p : sharedptr object
 *  result:
 *      return the sharedptr with added ref-count
 *  remark:
 *      if p is scopedptr,return normal and not added ref-count
 */
__SpxPublic__ __SpxSmartptrType__(void *) spx_allocator_smartptr_ref(void *p) { /*{{{*/
    spx_entry_smartptr_t *entry = spx_container_of(p, spx_entry_smartptr_t, buf);
    if (SpxEntrySmartptr == entry->flag) {
        entry->ref_count++;
    }
    return p;
} /*}}}*/

/*
 * decr sharedptr ref-count then if ref-count of smartptr is zero,free it.
 * paras:
 *  p:smartptr object
 *
 */
__SpxPublic__ void
spx_allocator_smartptr_unref(void *p) { /*{{{*/
    spx_allocator_smartptr_cleanup(p);
} /*}}}*/

/*
__SpxPublic__ __SpxSmartptrType__(void *) spx_allocator_smartptr_resize(
                                                    spx_log_fn *log,
                                                    void *p,
                                                   const int size,
                                                   const bool_t is_new_when_mulit_ref,
                                                   bool_t *is_mulit_ref,
                                                   bool_t *is_new,
                                                   spx_ret_t *ret)
{
    spx_allocator_smartptr_metadata_t *md = spx_allocator_smartptr_get_metadata(p);
    if (size <= md->bsize)
    {
        return p;
    }
    int usize = md->bsize - md->freesize;
    i32_t blen = spx_sys_align(size);
    int hlen = sizeof(spx_allocator_smartptr_metadata_t);
    if (scoped == md->kind || normal == md->kind)
    {
        spx_entry_smartptr_t *ptr = realloc((spx_entry_smartptr_t *)md, hlen + blen);
        spx_assert(ptr);
        spx_allocator_smartptr_metadata_t *md_new = (spx_allocator_smartptr_metadata_t *)ptr;
        md_new->bsize = blen;
        md_new->freesize = blen - usize;
        *is_new = false;
        return ptr->p;
    }
    if (shared == md->kind)
    {
        *is_mulit_ref = 1 < md->ref_count;
        if (*is_mulit_ref && is_new_when_mulit_ref)
        {
            // new a shared smart pointer
            void *new = spx_allocator_smartptr_wrap(shared, blen, md->dtor, ret);
            spx_log_exp_rtn(log,SpxLogError,!new,NULL,*ret,
                        "new shared smartptr.");
            memcpy(new, p, usize);
            spx_allocator_smartptr_update_freesize(new, usize);
            *is_new = true;
            return new;
        }
        else
        {
            // only realloc the ptr and not add the ref_count
            spx_entry_smartptr_t *ptr = realloc((spx_entry_smartptr_t *)md, hlen + blen);
            *ret = (spx_ret_t) errno;
            spx_log_exp_rtn(log,SpxLogError,!ptr,NULL,*ret,
                            "realloc smartptr.");
            spx_allocator_smartptr_metadata_t *md_new = (spx_allocator_smartptr_metadata_t *)ptr;
            md_new->bsize = blen;
            md_new->freesize = blen - usize;
            *is_new = false;
            return ptr->p;
        }
    }
    return NULL;
}

*/
