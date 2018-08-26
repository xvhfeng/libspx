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

#include "spx_ctype.h"
#include "spx_mfunc.h"
#include "spx_ret.h"

typedef struct spx_spinlock_s spx_spinlock_t;

typedef bool_t(spx_spinlock_lock_fn)(spx_spinlock_t *lock,
                                     const sint_t owner,
                                     const i32_t yield_times,
                                     const i32_t max_trytimes,
                                     spx_ret_t *ret);

typedef bool_t(spx_spinlock_trylock_fn)(spx_spinlock_t *lock,
                                        const sint_t owner,
                                        spx_ret_t *ret);

typedef bool_t(spx_spinlock_unlock_fn)(spx_spinlock_t *lock,
                                       const sint_t owner,
                                       spx_ret_t *ret);

typedef bool_t(spx_spinlock_unlock_fn)(spx_spinlock_t *lock,
                                       const sint_t owner,
                                       spx_ret_t *ret);

typedef sint_t(spx_spinlock_reset_fn)(spx_spinlock_t *lock,
                                      spx_ret_t *ret);

typedef void(spx_spinlock_dtor_fn)(spx_spinlock_t **lock);

struct spx_spinlock_s {
    char name[ 64 ];
    sint_t owner;
    spx_spinlock_lock_fn *lock;
    spx_spinlock_trylock_fn *trylock;
    spx_spinlock_unlock_fn *unlock;
    spx_spinlock_reset_fn *reset;
    spx_spinlock_dtor_fn *dtor;
};

__SpxPublic__ spx_spinlock_t *
spx_spinlock_new(const char *name,
                 spx_ret_t *ret);

#ifdef __cplusplus
}
#endif
#endif
