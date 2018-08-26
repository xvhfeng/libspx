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
 *       Filename:  spx_spinlock.c
 *        Created:  2018年08月09日 20时24分36秒
 *         Author:  Seapeak.Xu (www.94geek.com), xvhfeng@gmail.com
 *        Company:  Tencent Literature
 *         Remark:
 *
 ***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "spx_atomic.h"
#include "spx_ctype.h"
#include "spx_mfunc.h"
#include "spx_process.h"
#include "spx_ret.h"
#include "spx_spinlock.h"

#define SpxSpinLockNormal 0

__SpxPrivate__ void
spx_spinlock_free(spx_spinlock_t **lock);

__SpxPrivate__ bool_t
spx_spinlock_lock(spx_spinlock_t *lock,
                  const sint_t owner,
                  const i32_t yield_times,
                  const i32_t max_trytimes,
                  spx_ret_t *ret);

__SpxPrivate__ bool_t
spx_spinlock_trylock(spx_spinlock_t *lock,
                     const sint_t owner,
                     spx_ret_t *ret);

__SpxPrivate__ bool_t
spx_spinlock_unlock(spx_spinlock_t *lock,
                    const sint_t owner,
                    spx_ret_t *ret);

__SpxPublic__ spx_spinlock_t *
spx_spinlock_new(const char *name,
                 spx_ret_t *ret) { /*{{{*/
    spx_spinlock_t *spin = calloc(sizeof(*spin), 1);
    memcpy(spin->name, name, SpxMin(strlen(name), 63));
    spin->lock = spx_spinlock_lock;
    spin->trylock = spx_spinlock_trylock;
    spin->unlock = spx_spinlock_unlock;
    spin->reset = spx_spinlock_reset;
    spin->dtor = spx_spinlock_free;
    return spin;
} /*}}}*/

__SpxPrivate__ void
spx_spinlock_free(spx_spinlock_t **lock) { /*{{{*/
    if (NULL != lock && NULL != *lock) {
        free(*lock);
        *lock = NULL;
    }
} /*}}}*/

__SpxPrivate__ bool_t
spx_spinlock_lock(spx_spinlock_t *lock,
                  const sint_t owner,
                  const i32_t yield_times,
                  const i32_t max_trytimes,
                  spx_ret_t *ret) { /*{{{*/
    int i = 0;
    for (; i <= max_trytimes; ++i) {
        if (spx_atomic_iscas(lock->owner, SpxSpinLockNormal, owner)) {
            return true;
        }
        if (0 == i % yield_times) {
            spx_sched_yield( );
        }
    }
    *ret = SpxRet_SpinkLockBusy;
    return false;
} /*}}}*/

__SpxPrivate__ bool_t
spx_spinlock_trylock(spx_spinlock_t *lock,
                     const sint_t owner,
                     spx_ret_t *ret) { /*{{{*/
    if (spx_atomic_iscas(lock->owner, SpxSpinLockNormal, owner)) {
        return true;
    }
    *ret = SpxRet_SpinkLockBusy;
    return false;
} /*}}}*/

__SpxPrivate__ bool_t
spx_spinlock_unlock(spx_spinlock_t *lock,
                    const sint_t owner,
                    spx_ret_t *ret) { /*{{{*/
    if (spx_atomic_iscas(lock->owner, owner, SpxSpinLockNormal)) {
        return true;
    }
    *ret = SpxRet_SpinLockFree;
    return false;
} /*}}}*/

__SpxPrivate__ sint_t
spx_spinlock_reset(spx_spinlock_t *lock,
                   spx_ret_t *ret) { /*{{{*/
    if (0 != lock->owner) {
        return spx_atomic_tas(lock->owner, 0);
    }
    *ret = SpxRet_SpinLockFree;
    return -1;
} /*}}}*/
