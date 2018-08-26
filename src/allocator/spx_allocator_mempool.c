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
 *       Filename:  spx_mpool.c
 *        Created:  2016/08/22 17时36分31秒
 *         Author:  Seapeak.Xu (www.94geek.com), xvhfeng@gmail.com
 *        Company:  Tencent Literature
 *         Remark:
 *
 ***********************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "_spx_log.h"
#include "spx_allocator.h"
#include "spx_ctype.h"
#include "spx_klist.h"
#include "spx_mfunc.h"

/*
 * alloc a new chunk and add it to pool
 * paras:
 *  allocator : the memory allocator
 * result:
 *  return code
 */
__SpxPrivate__ spx_ret_t
spx_allocator_mempool_add_chunk(spx_log_fn *log,
                                spx_allocator_mempool_t *pool) { /*{{{*/
    spx_ret_t ret = SpxRet_OK;
    spx_errno_clear;
    spx_allocator_mempool_chunk_t *chunk
        = calloc(sizeof(spx_allocator_mempool_chunk_t) + pool->chunksizeB);
    ret = (spx_ret_t) errno;
    spx_log_exp_rtn(log, SpxLogError, !chunk, ret, ret,
                    "new chunk for pool->%s.", pool->name);
    int headsizeB = spx_sys_align(sizeof(spx_allocator_mempool_chunk_t));
    chunk->busysizeB = 0;
    spx_klist_add_tail(&(chunk->chain), &(pool->chunks));
    spx_atomic_incr(pool->chunks_count);
    pool->curr_chunk = chunk;
    return ret;
} /*}}}*/

__SpxPrivate__ __SpxInline__ spx_ret_t
spx_allocator_mempool_free_chunks(spx_allocator_t *allocator) { /*{{{*/
    spx_allocator_mempool_t *pool = allocator->pool;
    spx_allocator_mempool_chunk_t *pos = NULL;
    spx_allocator_mempool_chunk_t *tmp = NULL;
    spx_klist_for_each_entry_safe(pos, tmp, spx_allocator_mempool_chunk_t, &(pool->chunks), chain) {
        spx_klist_del(&(pos->chain));
        spx_atomic_decr(pool->chunks_count);
        free(pos);
    }
    return SpxRet_OK;
} /*}}}*/

__SpxPrivate__ spx_ret_t
spx_allocator_mempool_free(spx_allocator_mempool_t **pool) { /*{{{*/
    spx_allocator_mempool_free_chunks(log, *pool);
    free(*pool);
    *pool = NULL;
    return SpxRet_OK;
} /*}}}*/

__SpxPrivate__ spx_ret_t
spx_allocator_mempool_reset(spx_log_fn *log,
                            spx_allocator_mempool_t *pool) { /*{{{*/
    spx_allocator_mempool_free_chunks(log, pool);
    spx_allocator_mempool_add_chunk(log, pool);
    return SpxRet_OK;
} /*}}}*/

__SpxPrivate__ string_t
spx_allocator_mempool_debug(spx_allocator_t *allocator) { /*{{{*/
    return NULL;
} /*}}}*/

/*
 * new a new pooling entry from mempool chunk
 * paras:
 *  alloc : memory allocator
 *  sizeB : pooling entry size,unit is byte
 *  ret : out argument,return-code
 * result:
 *  the new pooling entry
 */
__SpxPrivate__ __SpxInline__ void *
spx_allocator_mempool_pooling_new(void *alloc,
                                  const i32_t sizeB,
                                  spx_ret_t *ret) { /*{{{*/
    SpxCasep(spx_allocator_t, allocator, alloc);
    SpxCasep(spx_allocator_mempool_t, pool, allocator->pool);
    spx_allocator_mempool_chunk_t *chunk = pool->curr_chunk;
    i32_t headsizeB = spx_sys_align(sizeof(spx_allocator_mempool_chunk_t));

    i32_t aligned_sizeB = spx_sys_align(sizeof(spx_entry_t)) + spx_sys_align(sizeB);
    if (headsizeB + aligned_sizeB + chunk->busysizeB > pool->chunksizeB) {
        *ret = spx_allocator_mempool_add_chunk(log, pool);
        spx_log_exp_rtn(log, SpxLogError, !!(*ret), NULL, *ret,
                        "add chunk for mempool->%s.", pool->name);
    }

    char *ptr = chunk->begin + chunk->busysizeB;
    SpxCasep(spx_entry_t, entry, ptr);
    spx_entry_base_set(entry, aligned_sizeB, sizeB, SpxEntryPooling);
    chunk->busysizeB += aligned_sizeB;
    spx_atomic_incr(allocator->poolings_count);
    return entry->buf;
} /*}}}*/

/*
 * new a mempool
 * paras:
 *  log : logger callback function
 *  name : allocator's name
 *  pagesizeB : system pagesize,unit is byte
 *  chunksizeB : size of chunk in the mempool,unit is byte
 *  largersizeB : size of freedom entry min-size,unit is byte
 *  ret : out argument,retrun-code
 * result:
 *  the new mempool
 */
__SpxPublic__ spx_allocator_mempool_t *
spx_allocator_mempool_new(spx_log_fn *log,
                          const char *name,
                          const int pagesizeB,
                          const int chunksizeB,
                          const int largersizeB,
                          spx_ret_t *ret) { /*{{{*/
    spx_errno_clear;
    spx_allocator_mempool_t *pool = calloc(sizeof(spx_allocator_mempool_t), sizeof(char));
    *ret = (spx_ret_t) errno;
    spx_log_exp_rtn(log, SpxLogError, pool, NULL, *ret,
                    "new mempool->%s.", name);
    pool->chunksizeB = spx_align(chunksizeB, pagesizeB);

    spx_klist_init_head(&(pool->chunks));
    *ret = spx_allocator_mempool_add_chunk(log, pool);
    spx_log_exp_goto(log, SpxLogError, !(*ret), r1, *ret,
                     "add chunk to pool->%s.", name);

    pool->self_free = spx_allocator_mempool_free;
    pool->self_reset = spx_allocator_mempool_reset;
    pool->self_debug = spx_allocator_mempool_debug;
    return pool;
r1:
    spx_allocator_mempool_free(log, &pool);
    return NULL;
} /*}}}*/

__SpxPublic__ void *
spx_allocator_mempool_entry_new(void *alloc,
                                const i32_t sizeB,
                                spx_ret_t *ret) { /*{{{*/
    if (0 >= sizeB) {
        *ret = SpxRet_ArgumentError;
        return NULL;
    }
    SpxCasep(spx_allocator_t, allocator, alloc);
    spx_allocator_mempool_t *pool = allocator->pool;

    if (sizeB >= pool->largersizeB) {
        return spx_entry_freedom_new(alloc, sizeB, ret);
    }
    return spx_allocator_mempool_pooling_new(alloc, sizeB, ret);
} /*}}}*/
