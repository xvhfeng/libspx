/***********************************************************************
 *                              _ooOoo_
 *                             o8888888o
 *                             88" . "88
 *                             (| -_- |)
 *                              O\ = /O
 *                          ____/`---'\____
 *                        .   ' \\| |// `.
 *                         / \\||| : |||// \
 *                       / _||||| -:- |||||- \
 *                         | | \\\ - /// | |
 *                       | \_| ''\---/'' | |
 *                        \ .-\__ `-` ___/-. /
 *                     ___`. .' /--.--\ `. . __
 *                  ."" '< `.___\_<|>_/___.' >'"".
 *                 | | : `- \`.;`\ _ /`;.`/ - ` : | |
 *                   \ \ `-. \_ __\ /__ _/ .-` / /
 *           ======`-.____`-.___\_____/___.-`____.-'======
 *                              `=---='
 *           .............................................
 *                    佛祖镇楼                  BUG辟易
 *            佛曰:
 *                    写字楼里写字间，写字间里程序员；
 *                    程序人员写程序，又拿程序换酒钱。
 *                    酒醒只在网上坐，酒醉还来网下眠；
 *                    酒醉酒醒日复日，网上网下年复年。
 *                    但愿老死电脑间，不愿鞠躬老板前；
 *                    奔驰宝马贵者趣，公交自行程序员。
 *                    别人笑我忒疯癫，我笑自己命太贱；
 *                    不见满街漂亮妹，哪个归得程序员？
 * ==========================================================================
 *
 * this software or lib may be copied only under the terms of the gnu general
 * public license v3, which may be found in the source kit.
 *
 *       Filename:  spx_spinlock.h
 *        Created:  2018年08月09日 20时18分50秒
 *         Author:  Seapeak.Xu (www.94geek.com), xvhfeng@gmail.com
 *        Company:  Tencent Literature
 *         Remark:
 *
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#ifndef _SPX_SPINLOCK_H_
#define _SPX_SPINLOCK_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "spx_allocator.h"
#include "spx_ctype.h"
#include "spx_mfunc.h"
#include "spx_ret.h"

struct spx_spinlock_s {
    /*
         * spinlock's name
         * */
    char name[ 64 ];
    /*
         * spinlock's owner,who get spinlock,then set owner to it's id
         */
    sint_t owner;
};

/*
     * new a spinlock with spinlock's name
     *
     * return:
     *  the spinlock object
     *
     * name:
     *   spinlock's name,max length is 64 include '\0'
     * ret :
     *   spx_ret_t value,if no memory for spinlock,set it
     */
__SpxPublic__ spx_spinlock_t *
spx_spinlock_new(spx_allocator_t *allocator,
                 const char *name,
                 spx_ret_t *ret);

/*
 * init add spinlock with lock's name
 */
__SpxPublic__ void
spx_spinlock_init(spx_spinlock_t *lock,
                  const char *name);

/*
     * free the spinlock,the spin lock from spx_spinlock_new
     * */
__SpxPublic__ void
spx_spinlock_free(spx_allocator_t *allocator,
                  spx_spinlock_t **lock);

/*
     * get spin lock if success return true
     *
     * return:
     *  if get lock success,return true, else return false
     *
     * lock :
     *   spx_spinlock_t object
     * owner :
     *   get locker owner,maybe is thread-id or process-id
     * yield_times :
     *   if not get lock,try yield-times then yield the cpu,
     *   if value less 0,use default value 3
     * max_trytimes :
     *   try get spinlock max times,
     *   if value is 0,means no max-times,try to get lock return
     *   if value less than 0,use dafault 30;if over 30,return false
     * ret :
     *   spx_ret_t value,if not get lock,set it
     *
     */
__SpxPublic__ bool_t
spx_spinlock_lock(spx_spinlock_t *lock,
                  const sint_t owner,
                  const i32_t yield_times,
                  const i32_t max_trytimes,
                  spx_ret_t *ret);

/*
     * try to get spinlock,get lock try once only
     *
     * return:
     *  if try get lock success,return true, else return false
     *
     * lock :
     *   spx_spinlock_t object
     * owner :
     *   get locker owner,maybe is thread-id or process-id
     * ret :
     *   spx_ret_t value,if not get lock,set it
     *
     */
__SpxPublic__ bool_t
spx_spinlock_trylock(spx_spinlock_t *lock,
                     const sint_t owner,
                     spx_ret_t *ret);

/*
     * unlock the spinlock
     * if lock's owner is not equ owner(argument),unlock fail.
     *
     * return:
     *  if unlock success,return true, else return false
     *
     * lock :
     *   spx_spinlock_t object
     * owner :
     *   get locker owner,maybe is thread-id or process-id
     * ret :
     *   spx_ret_t value,if not unlock spinlock,set it
     *
     */
__SpxPublic__ bool_t
spx_spinlock_unlock(spx_spinlock_t *lock,
                    const sint_t owner,
                    spx_ret_t *ret);

/*
     * reset the spinlock when it was dead-lock
     * force set owner to 0
     * dead-lock:means owner is dead,but lock's owner not be unlock.
     *           maybe thread was over or process was kiiled
     * return :
     *  spinlock's owner
     *  if return -1,and ret == SpxRet_LockFree,spinlock is free,not to reset
     *
     * lock :
     *   spx_spinlock_t object
     * ret :
     *   spx_ret_t value,if not reset spinlock,set it
     */
__SpxPublic__ sint_t
spx_spinlock_reset(spx_spinlock_t *lock,
                   spx_ret_t *ret);

#ifdef __cplusplus
}
#endif
#endif
