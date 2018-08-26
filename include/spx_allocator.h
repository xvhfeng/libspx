#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "_spx_log.h"
#include "spx_chain.h"
#include "spx_ctype.h"
#include "spx_debugger.h"
#include "spx_klist.h"
#include "spx_mfunc.h"
#include "spx_ret.h"

/*
 * normal entry
 */
#define SpxEntryNormal 0x01

/*
 * cleanup entry
 * this entry has a dtor function that is auto called
 */
#define SpxEntryCleanup 0x02

/*
 * pooling normal entry
 */
#define SpxEntryPooling 0x04

/*
 * smartptr entry
 */
#define SpxEntrySmartptr 0x08

typedef spx_ret_t(spx_entry_dtor_fn)(void *alloc,
                                     void **ptr);

/*
 * smartptr destory function callback
 * dtor function destory the item and free the smartptr self
 * */
typedef void(spx_smartptr_dtor_fn)(void **p);

typedef void (*spx_entry_ctor_fn)(void *alloc,
                                  const i32_t sizeB,
                                  spx_ret_t *ret);

typedef void (*spx_cleanup_ctor_fn)(void *alloc,
                                    const i32_t sizeB,
                                    spx_entry_dtor_fn *dtor,
                                    spx_ret_t *ret);

typedef void (*spx_smartptr_ctor_fn)(const i32_t sizeB,
                                     spx_smartptr_dtor_fn *dtor,
                                     spx_ret_t *ret);

typedef spx_ret_t(spx_allocator_reset_fn)(spx_allocator_t *allocator);

typedef void(spx_allocator_free_fn)(spx_allocator_t **allocator);

typedef string_t(spx_allocator_debug_fn)(spx_allocator_t *allocator);

/*
 * all entry base property
 * 24 byte in the head in x64
 * 12 byte in the head in x32
 * remark:
 *  len : the totalsize include the head length
 *  busylen : the busy context in the buf,not include the head length
 *  flag : style for entry
 */
#define SpxEntryBase \
    sint_t len;      \
    sint_t busylen;  \
    sint_t flag;     \
    char buf[ 0 ]
/*
 * all entry base type
 * or entry type using pooling
 */
typedef struct {
    SpxEntryBase;
} spx_entry_t;

/*
 * freedom entry or larger entry using pooling
 */
typedef struct {
    spx_klist_node_t chain;
    SpxEntryBase;
} spx_entry_freedom_t;

/*
 * cleanup entry
 */
typedef struct {
    spx_cleanup_dtor_fn *dtor;
    spx_klist_node_t chain;
    SpxEntryBase;
} spx_entry_cleanup_t;

/*
 * structor for smart pointer
 */
typedef struct spx_allocator_smartptr {
    spx_atomic_t sint_t ref_count;  // ref count for shared pointer
    spx_allocator_t *allocator;
    spx_smartptr_dtor_fn *dtor;  // destructor callback
    spx_klist_node_t chain;
    SpxEntryBase;
} spx_entry_smartptr_t;

typedef struct spx_allocator_mempool_chunk {
    spx_klist_node_t chain;
    spx_atomic_t i32_t busysizeB;
    char begin[ 0 ];
} spx_allocator_mempool_chunk_t;

typedef spx_ret_t(spx_allocator_mempool_reset_fn)(spx_allocator_mempool_t *mempool);
typedef spx_ret_t(spx_allocator_mempool_free_fn)(spx_allocator_mempool_t **mempool);
typedef spx_ret_t(spx_allocator_mempool_debug_fn)(spx_allocator_mempool_t *mempool);

typedef struct spx_allocator_mempool {
    spx_log_fn *log;
    i32_t chunksizeB;
    i32_t largersizeB;
    sint_t chunks_count;
    spx_klist_node_t chunks;
    spx_allocator_mempool_chunk_t *curr_chunk;

    struct spx_allocator_mempool *self;
    spx_allocator_mempool_reset_fn *self_reset;
    spx_allocator_mempool_free_fn *self_free;
    spx_allocator_mempool_debug_fn *self_debug;
} spx_allocator_mempool_t;

typedef struct spx_allocator {
    // allocator chain
    spx_klist_node_t chain;

    // chain for every kind entry
    spx_klist_node_t cleanups;
    spx_klist_node_t freedoms;
    spx_klist_node_t smartptrs;

    //count of every kind entry
    spx_atomic_t sint_t cleanups_count;
    spx_atomic_t sint_t freedoms_count;
    spx_atomic_t sint_t smartptrs_count;
    spx_atomic_t sint_t poolings_count;

    //memory use size of every kind entry
    spx_atomic_t sint_t cleanups_usesize;
    spx_atomic_t sint_t freedoms_usesize;
    spx_atomic_t sint_t smartptrs_usesize;

    spx_allocator_mempool_t *pool;
    spx_log_fn *log;
    spx_entry_ctor_fn *entry_new;
    spx_entry_dtor_fn *entry_free;
    spx_cleanup_ctor_fn *cleanup_new;
    spx_smartptr_ctor_fn *smartptr_new;
    char name[ 64 ];

    struct spx_allocator *self;
    //reset allocator self
    spx_allocator_reset_fn *self_reset;
    //free allocator self
    spx_allocator_free_fn *self_free;
    //debug
    spx_allocator_debug_fn *self_debug;

} spx_allocator_t;

/* set entry base property
 * ep : entry pointer
 * l : entry length
 * bl : entry busy length
 * f : entry flag
 */
#define spx_entry_base_set(ep, l, bl, f) \
    ep->len = l;                         \
    ep->busylen = bl;                    \
    ep->flag = f

/*
 * update busylen for entry with buf
 * paras:
 *  buf : entry buf pointer
 *  busylen : entry busylen
 */
__SpxPrivate__ __SpxInline__ void
spx_entry_update_busylen(const void *b, const sint_t busylen) { /*{{{*/
    spx_entry_t *entry = spx_container_of(b, spx_entry_t, buf);
    entry->busylen = busylen;
} /*}}}*/

/*
 * get entry base property
 * if out-parar(len,busylen,flag) is NULL,then not set value
 * */
__SpxPrivate__ __SpxInline__ void
spx_entry_get_base(const void *b,
                   sint_t *len,
                   sint_t *busylen,
                   sint_t *flag) { /*{{{*/
    spx_entry_t *entry = spx_container_of(b, spx_entry_t, buf);
    if (NULL != len) {
        *len = entry->len;
    }
    if (NULL != busylen) {
        *busylen = entry->busylen;
    }
    if (NULL != flag) {
        *flag = entry->flag;
    }
} /*}}}*/

/*
 * new entry with type
 *
 * return :
 *   object pointer to type t
 *
 * para:
 *      atr : allocator
 *      t : type without pointer
 *      ret : return code with spx_ret_t
 */
#define spx_entry_new(atr, t, ret) \
    ((t *) (atr)->entry_new((atr), sizeof(t), (ret)))
/*
 * new cleanup entry with type and cleanup's dtor
 *
 * return :
 *   object pointer to type t
 *
 * para:
 *      atr : allocator
 *      t : type without pointer
 *      dtor : cleanup entry dtor function
 *      ret : return code with spx_ret_t
 * */
#define spx_cleanup_new(atr, t, dtor, ret) \
    ((t *) (atr)->cleanup_new((atr), sizeof(t), (dtor), (ret)))

/*
 * new smartptr entry with type
 *
 * return :
 *   object pointer to type t
 *
 * para:
 *      atr : allocator
 *      t : type without pointer
 *      dtor : smartptr entry dtor function
 *      ret : return code with spx_ret_t
 * */
#define spx_smartptr_new(atr, t, dtor, ret) \
    ((t *) (atr)->smartptr_new((atr), sizeof(t), (dtor), (ret)))

/*
 * new entry with length in type
 *
 * return :
 *   object pointer to type t
 *
 * para:
 *      atr : allocator
 *      t : type without pointer
 *      len : entry size
 *      ret : return code with spx_ret_t
 *  */
#define spx_entry_newlen(atr, t, len, ret) \
    ((t *) (atr)->entry_new((atr), (len), (ret)))

/*
 * new cleanup entry with length in type
 *
 * return :
 *   object pointer to type t
 *
 * para:
 *      atr : allocator
 *      t : type without pointer
 *      len : entry size
 *      dtor : cleanup entry dtor function
 *      ret : return code with spx_ret_t
 *  */
#define spx_cleanup_newlen(atr, t, len, dtor, ret) \
    ((t *) (atr)->cleanup_new((atr), (len), (dtor), (ret)))

/*
 * new smartptr entry with length in type
 *
 * return :
 *   object pointer to type t
 *
 * para:
 *      atr : allocator
 *      t : type without pointer
 *      len : entry size
 *      dtor : smartptr entry dtor function
 *      ret : return code with spx_ret_t
 *  */
#define spx_smartptr_newlen(atr, t, len, dtor, ret) \
    ((t *) (atr)->smartptr_new((atr), (len), (dtor), (ret)))

/*
 * new entry with length in type
 *
 * return :
 *   object to type t
 *
 * para:
 *      atr : allocator
 *      t : type  pointer
 *      len : entry size
 *      ret : return code with spx_ret_t
 *  */
#define spx_entry_newlen_p(atr, t, len, ret) \
    ((t)(atr)->entry_new((atr), (len), (ret)))

/*
 * new cleanup entry with length in type
 *
 * return :
 *   object to type t
 *
 * para:
 *      atr : allocator
 *      t : type  pointer
 *      len : entry size
 *      dtor : cleanup entry dtor function
 *      ret : return code with spx_ret_t
 *  */
#define spx_cleanup_newlen_p(atr, t, len, dtor, ret) \
    ((t)(atr)->cleanup_new((atr), (len), (dtor), (ret)))

/*
 * new smartptr entry with length in type
 *
 * return :
 *   object to type t
 *
 * para:
 *      atr : allocator
 *      t : type  pointer
 *      len : entry size
 *      dtor : smartptr entry dtor function
 *      ret : return code with spx_ret_t
 *  */
#define spx_smartptr_newlen_p(atr, t, len, dtor, ret) \
    ((t)(atr)->smartptr_new((atr), (len), (dtor), (ret)))

/*
 * marking return type or argument type for function is the smartptr
 * so the function return shared smart pointer just only
 * expl:
 *  __SpxSmartptrType__(int * ) get_peope_count(__SpxSmartptrType__(int *) a) {
 *      spx_smartptr_t int *t = spx_shared_ptr(...);
 *      return t;
 *  }
 */
#define __SpxSmartptrType__(type) type

/*
 * define the qualifier for smart pointer variable
 * if you declare or define a smart pointer,use like:
 *  SpxSmart int *ssp = NULL;
 * or like:
 *  spx_smartptr_t struct people *p = spx_smartptr_new();
 */
#define spx_smartptr_t \
    __attribute__((cleanup(spx_allocator_smartptr_cleanup)))

/*
 * return the smart pointer
 * when return the smart pointer,use statement like:
 *  return SpxSmartReturn(smart-pointer);
 */
#define spx_smartptr_rtn(p) \
    ({                      \
        spx_smart_ref(p);   \
        p;                  \
    })

__SpxPublic__ void
    *
    spx_allocator_freedom_new(void *alloc,
                              const i32_t sizeB,
                              spx_ret_t *ret);

__SpxPublic__ void
    *
    spx_allocator_cleanup_new(void *alloc,
                              const i32_t sizeB,
                              spx_cleanup_dtor_fn *dtor,
                              spx_ret_t *ret);

__SpxPublic__ spx_ret_t
spx_allocator_entry_free(void *alloc, void **ptr);

/*
 * new a default allocator
 * para:
 *  log : logger
 *  name : allocator name,max length is 63
 *  ret : out argument, return-code
 * result:
 *  a new default allocator
 * */
__SpxPublic__ spx_allocator_t *
spx_allocator_new(spx_log_fn *log,
                  const char *name,
                  spx_ret_t *ret);

/*
 * new a pooling allocator
 * para:
 *  log : logger
 *  name : allocator name,max length is 63
 *  dalloc : default allocator,this value is &spx_allocator_default usually
 *  pagesizeB : system memory page size,unit is byte
 *  chunksizeB : chunk size that in the memory pooling
 *  largersizeB : the larger entry min size in the pooling
 *  ret : out argument, return-code
 * result:
 *  a new pooling allocator
 * */
__SpxPublic__ spx_allocator_mempool_t *
spx_allocator_mempool_new(spx_log_fn *log,
                          const char *name,
                          const int pagesizeB,
                          const int chunksizeB,
                          const int largersizeB,
                          spx_ret_t *ret);

__SpxPublic__ void
    *
    spx_allocator_mempool_entry_new(void *alloc,
                                    const i32_t sizeB,
                                    spx_ret_t *ret);

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
__SpxPublic__ __SpxSmartptrType__(void) * spx_allocator_smartptr_new(spx_allocator_t *allocator, const int sizeB, spx_smartptr_dtor_fn *dtor, spx_ret_t *ret);

/*
 * cleanup callback for smart pointer
 * it's called auto by the variable out of the scope
 * paras:
 *  p : the smart pointer
 *
 */
__SpxPublic__ void
spx_allocator_smartptr_cleanup(void **p);

/*
 * incr the referenced count for shared pointer
 * paras :
 *  p : the smart pointer
 * result :
 *  return the smart pointer
 */
__SpxPublic__ __SpxSmartptrType__(void) * spx_allocator_smartptr_ref(void *p);

/*
     * decr the referenced count fot smart pointer
     * the function use in the dtor for the member of smart pointer
     * when the decr the ref_count to 0,destory the smart pointer as member
     * paras:
     *  p : the smart pointer
     */
__SpxPublic__ void spx_allocator_smartptr_unref(void *p);

/*
     * free the smart pointer force
     */
__SpxPrivate__ __SpxInline__ void
spx_allocator_smartptr_free(void *p) { /*{{{*/
    union {
        void **rptr;
        void *ptr;
    } c = { .ptr = p };
    spx_entry_smartptr_t *ptr = spx_container_of(p, spx_entry_smartptr_t, buf);
    free(ptr);
    ptr = NULL;
    *(c.rptr) = NULL;
} /*}}}*/

/*
 * get allocator from smartptr hide-header
 */
__SpxPrivate__ __SpxInline__ spx_allocator_t *
spx_allocator_from_smartptr(void *ptr) {
    spx_entry_smartptr_t *smartptr = spx_containor_of(ptr, spx_entry_smartptr_t, buf);
    return smartptr->allocator;
}
