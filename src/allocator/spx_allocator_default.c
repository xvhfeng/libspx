#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "spx_allocator.h"
#include "spx_ctype.h"
#include "spx_mfunc.h"
#include "spx_ret.h"

__SpxPublic__ void *
spx_allocator_freedom_new(void *alloc,
                          const i32_t sizeB,
                          spx_ret_t *ret) { /*{{{*/
    if (0 > sizeB) {
        *ret = SpxRet_ArgumentError;
        return null;
    }
    spx_allocator_t *allocator = (spx_allocator_t *) alloc;
    i32_t totalsizeB = sizeof(spx_entry_freedom_t) + spx_sys_align(sizeB);
    spx_errno_clear;
    spx_entry_freedom_t *entry = calloc(sizeof(char), totalsizeB);
    if (null == entry) {
        *ret = (spx_ret_t) errno;
        return null;
    }
    spx_entry_base_set(entry, totalsizeB, sizeB, SpxEntryFreedom);
    spx_klist_add_tail(&(allocator->freedoms), &(entry->chain));
    spx_atomic_add(allocator->freedoms_usesize, totalsizeB);
    spx_atomic_incr(allocator->freedoms_count);
    return entry->buf;
} /*}}}*/

__SpxPublic__ void *
spx_allocator_cleanup_new(void *alloc,
                          const i32_t sizeB,
                          spx_cleanup_dtor_fn *dtor,
                          spx_ret_t *ret) { /*{{{*/
    if (0 > sizeB) {
        *ret = SpxRet_ArgumentError;
        return null;
    }

    spx_allocator_t *allocator = (spx_allocator_t *) alloc;
    i32_t totalsizeB = sizeof(spx_entry_cleanup_t) + spx_sys_align(sizeB);
    spx_errno_clear;
    spx_entry_cleanup_t *entry = calloc(sizeof(char), totalsizeB);
    if (null == entry) {
        *ret = (spx_ret_t) errno;
        return null;
    }
    spx_entry_base_set(entry, totalsizeB, sizeB, SpxEntryCleanup);
    entry->dtor = dtor;
    spx_klist_add_tail(&(allocator->cleanups), &(entry->chain));
    spx_atomic_add(allocator->cleanups_usesize, totalsizeB);
    spx_atomic_incr(allocator->cleanups_count);

    return entry->buf;
} /*}}}*/

__SpxPublic__ spx_ret_t
spx_allocator_entry_free(void *alloc, void **ptr) { /*{{{*/
    if (null == ptr || null == *ptr)
        return SpxRet_ArgumentError;
    spx_allocator_t *allocator = (spx_allocator_t *) alloc;
    spx_entry_t *entry = spx_container_of(*ptr, spx_entry_t, buf);
    switch (entry->flag) {
        case (SpxEntryFreedom): {
            spx_entry_freedom_t *freedom = spx_container_of(*ptr, spx_entry_freedom_t, buf);
            spx_klist_del(&(freedom->chain));
            spx_atomic_decr(allocator->freedoms_count);
            spx_atomic_sub(allocator->freedoms_usesize, freedom->len);
            free(freedom);
            *ptr = NULL;
            break;
        }
        case (SpxEntryCleanup): {
            spx_entry_cleanup_t *cleanup = spx_container_of(*ptr, spx_entry_cleanup_t, buf);
            if (NULL != cleanup->dtor) {
                cleanup->dtor(*ptr);
            }
            spx_klist_del(&(cleanup->chain));
            spx_atomic_decr(allocator->cleanups_count);
            spx_atomic_sub(allocator->cleanups_usesize, cleanup->len);
            free(cleanup);
            *ptr = NULL;
            break;
        }
    }
    return SpxRet_OK;
} /*}}}*/
