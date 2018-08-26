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

#include "spx_allocator.h"
#include "spx_atomic.h"
#include "spx_ctype.h"
#include "spx_mfunc.h"
#include "spx_process.h"
#include "spx_ret.h"
#include "spx_spinlock.h"

#define SpxSpinLockNormal 0
#define SpxSpinLockYieldTimes 3
#define SpxSpinLockMaxTrytimes 30

__SpxPublic__ spx_spinlock_t *
spx_spinlock_new(spx_allocator_t *allocator,
                 const char *name,
                 spx_ret_t *ret) { /*{{{*/
    spx_spinlock_t *spin = spx_entry_new(allocator,
                                         spx_spinlock_t,
                                         ret);
    spx_log_exp_rtn(allocator->log, SpxLogError,
                    !spin, NULL, ret,
                    "new spinlock is fail.");

    spx_spinlock_init(spin, name);
    return spin;
} /*}}}*/

__SpxPublic__ void
spx_spinlock_init(spx_spinlock_t *lock,
                  const char *name) { /*{{{*/
    memcpy(spin->name, name, SpxMin(strlen(name), 63));
    lock->owner = 0;
    return;
} /*}}}*/

__SpxPublic__ void
spx_spinlock_free(spx_spinlock_t **lock) { /*{{{*/
    if (NULL != lock && NULL != *lock) {
        free(*lock);
        *lock = NULL;
    }
} /*}}}*/

__SpxPublic__ bool_t
spx_spinlock_lock(spx_spinlock_t *lock,
                  const sint_t owner,
                  const i32_t yield_times,
                  const i32_t max_trytimes,
                  spx_ret_t *ret) { /*{{{*/
    int i = 0;
    if (0 >= yield_times) {  //must have yield time,less than 0,use default 3
        yield_times = SpxSpinLockYieldTimes;
    }
    if (0 > max_trytimes) {  // less than 0 ,use default 30
        max_trytimes = SpxSpinLockMaxTrytimes;
    }
    while (true) {
        if (spx_atomic_trycas(lock->owner, SpxSpinLockNormal, owner)) {
            return true;
        }
        if (0 == ++i % yield_times) {
            spx_sched_yield( );
        }
        if (0 == max_trytimes) {  //try to get lock
            continue;
        }
        if (i == max_trytimes) {
            *ret = SpxRet_LocksBusy;
            break;
        }
    }
    return false;
} /*}}}*/

__SpxPublic__ bool_t
spx_spinlock_trylock(spx_spinlock_t *lock,
                     const sint_t owner,
                     spx_ret_t *ret) { /*{{{*/
    if (spx_atomic_trycas(lock->owner, SpxSpinLockNormal, owner)) {
        return true;
    }
    *ret = SpxRet_LocksBusy;
    return false;
} /*}}}*/

__SpxPublic__ bool_t
spx_spinlock_unlock(spx_spinlock_t *lock,
                    const sint_t owner,
                    spx_ret_t *ret) { /*{{{*/
    if (spx_atomic_iscas(lock->owner, owner, SpxSpinLockNormal)) {
        return true;
    }
    if (0 == lock->owner) {  //not exist onwer,free lock
        return true;
    }
    *ret = SpxRet_DeadLock;
    return false;
} /*}}}*/

__SpxPublic__ sint_t
spx_spinlock_reset(spx_spinlock_t *lock,
                   spx_ret_t *ret) { /*{{{*/
    if (0 != lock->owner) {
        return spx_atomic_tas(lock->owner, SpxSpinLockNormal);
    }
    *ret = SpxRet_LockFree;
    return -1;
} /*}}}*/
