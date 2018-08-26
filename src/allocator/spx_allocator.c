#include <stdio.h>
#include <stdlib.h>

#include "_spx_log.h"
#include "spx_allocator.h"
#include "spx_ctype.h"
#include "spx_mfunc.h"
#include "spx_ret.h"

__SpxPublic__ spx_allocator_t *
spx_allocator_new(spx_log_fn *log,
                  const char *name,
                  spx_ret_t *ret) { /*{{{*/
    size_t namelen = strlen(name);
    if (0 == namelen) {
        *ret = SpxRet_ArgumentError;
        return NULL;
    }
    spx_errno_clear;
    spx_allocator_t *alloc = calloc(sizeof(spx_allocator_t), sizeof(char));
    *ret = (spx_ret_t) errno;
    spx_log_exp_rtn(log, SpxLogError, !alloc, NULL, *ret,
                    "new allocator->%s.", name);

    alloc->pool = NULL;
    alloc->log = log;
    alloc->entry_new = spx_allocator_freedom_new;
    alloc->entry_free = spx_allocator_freedom_free;
    alloc->cleanup_new = spx_allocator_cleanup_new;
    alloc->smartptr_new = spx_allocator_smartptr_new;
    alloc->self_reset = spx_allocator_reset;
    alloc->self_free = spx_allocator_free;
    alloc->self_debug = spx_allocator_debug;
    memcpy(allocator->name, name, SpxMin(63, namelen));
    alloc->self = alloc;
    spx_klist_init_head(&(pool->freedoms));
    spx_klist_init_head(&(pool->cleanups));
    return alloc;
} /*}}}*/

__SpxPublic__ spx_allocator_t *
spx_allocator_new_pooing(spx_log_fn *log,
                         const char *name,
                         const i32_t pagesizeB,
                         const i32_t chunksizeB,
                         const i32_t largersizeB,
                         spx_ret_t *ret) { /*{{{*/
    size_t namelen = strlen(name);
    if (0 == namelen) {
        *ret = SpxRet_ArgumentError;
        return NULL;
    }
    spx_errno_clear;
    spx_allocator_t *alloc = calloc(sizeof(spx_allocator_t), sizeof(char));
    *ret = (spx_ret_t) errno;
    spx_log_exp_rtn(log, SpxLogError, !alloc, NULL, *ret,
                    "new allocator->%s.", name);

    spx_allocator_mempool_t *pool = spx_allocator_mempool_alloc(log, name,
                                                                &spx_allocator_default,
                                                                pagesizeB, chunksizeB,
                                                                largersizeB, ret);
    spx_log_exp_goto(log, SpxLogError, !pool, fail, *ret,
                     "new mempool->%s.", name);

    alloc->log = log;
    alloc->pool = pool;
    alloc->entry_new = spx_allocator_mempool_alloc_entry;
    alloc->entry_free = spx_allocator_mempool_free_entry;
    alloc->cleanup_new = spx_allocator_cleanup_new;
    alloc->smartptr_new = spx_allocator_smartptr_new;
    alloc->self_reset = spx_allocator_reset;
    alloc->self_free = spx_allocator_free;
    alloc->self_debug = spx_allocator_debug;
    memcpy(alloc->name, name, SpxMin(63, namelen));
    alloc->self = alloc;
    spx_klist_init_head(&(pool->freedoms));
    spx_klist_init_head(&(pool->cleanups));
    return alloc;
fail:
    free(alloc);
    return NULL;
} /*}}}*/

__SpxPrivate__ spx_ret_t
spx_allocator_reset(spx_allocator_t *allocator) { /*{{{*/
    spx_allocator_free_freedoms(allocator);
    spx_allocator_free_cleanups(allocator);
    spx_ret_t ret = spx_allocator_mempool_add_chunk(allocator->log, allocator->pool);
    spx_log_exp_rtn(allocator->log,SpxLogError,ret
} /*}}}*/

__SpxPrivate__ string_t
spx_allocator_debug(spx_allocator_t *allocator) { /*{{{*/
    return NULL;
} /*}}}*/

__SpxPrivate__ void
spx_allocator_free(spx_allocator_t **allocator) { /*{{{*/
    if (NULL == act || NULL == *act)
        return;

    spx_klist_del(&((*allocator)->chain));
    spx_allocator_free_freedoms(*allocator);
    spx_allocator_free_cleanups(*allocator);
    if (NULL != (*allocator)->pool) {
        spx_allocator_mempool_free((*allocator)->pool);
    }
    free(*act);
    *act = NULL;
} /*}}}*/

__SpxPrivate__ __SpxInline__ spx_ret_t
spx_allocator_free_freedoms(spx_allocator_t *allocator) { /*{{{*/
    spx_entry_freedom_t *pos = NULL;
    spx_entry_freedom_t *tmp = NULL;
    spx_klist_for_each_entry_safe(pos, tmp, spx_entry_freedom_t, &(allocator->freedoms), chain) {
        allocator->entry_free(allocator, pos);
    }
    return SpxRet_OK;
} /*}}}*/

__SpxPrivate__ __SpxInline__ spx_ret_t
spx_allocator_free_cleanups(spx_allocator_t *allocator) { /*{{{*/
    spx_entry_cleanup_t *pos = NULL;
    spx_entry_cleanup_t *tmp = NULL;
    spx_klist_for_each_entry_safe(pos, tmp, spx_entry_cleanup_t, &(allocator->cleanups), chain) {
        allocator->entry_free(allocator, pos);
    }
    return SpxRet_OK;
} /*}}}*/
