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
 *       Filename:  spx_atomic.h
 *        Created:  2018/08/02 14时14分36秒
 *         Author:  Seapeak.Xu (www.94geek.com), xvhfeng@gmail.com
 *        Company:  Tencent Literature
 *         Remark:
 *
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#ifndef _SPX_ATOMIC_H_
#define _SPX_ATOMIC_H_
#ifdef __cplusplus
extern "C" {
#endif

/*
     * v + a,return v before added
     */
#define spx_atomic_add_p(ptr, p) __sync_add_and_fetch(ptr, p)

/*
     * v - s,return v before subed
     */
#define spx_atomic_sub_p(ptr, p) __sync_sub_and_fetch(ptr, p)

/*
     * v + 1 ,return v before added
     */
#define spx_atomic_incr_p(ptr) __sync_add_and_fetch(ptr, 1)

/*
     * v - 1,return v after subed
     */
#define spx_atomic_decr_p(ptr) __sync_sub_and_fetch(ptr, 1)

/*
     * v + a,return v after added
     */
#define spx_atomic_lazy_add_p(ptr, p) __sync_fetch_and_add(ptr, p)

/*
     * v - s,return v after subed
     */
#define spx_atomic_lazy_sub_p(ptr, p) __sync_fetch_and_sub(ptr, p)

/*
     * v + 1 ,return v after added
     */
#define spx_atomic_lazy_incr_p(ptr) __sync_fetch_and_add(ptr, 1)

/*
     * v - 1,return v after subed
     */
#define spx_atomic_lazy_decr_p(ptr) __sync_fetch_and_sub(ptr, 1)

/*
     * set v to 0
     * */
#define spx_atomic_release_p(ptr) __sync_lock_release(ptr)

/*
     * set n to v and return v
     * */
#define spx_atomic_tas_p(ptr, p) __sync_lock_test_and_set(ptr, p)

/*
     * try set n to v when o == v,return true if set is success
     * */
#define spx_atomic_trycas_p(ptr, o, p) __sync_bool_compare_and_swap(ptr, o, p)

/*
     * try set n to v when o == v,return v if set is success
     * */
#define spx_atomic_cas_p(ptr, o, p) __sync_val_compare_and_swap(ptr, o, p)

/*
     * v + a,return v before added
     */
#define spx_atomic_add(v, a) __sync_add_and_fetch(&(v), a)

/*
     * v - s,return v before subed
     */
#define spx_atimoc_sub(v, s) __sync_sub_and_fetch(&(v), s)

/*
     * v + 1 ,return v before added
     */
#define spx_atomic_incr(v) __sync_add_and_fetch(&(v), 1)

/*
     * v - 1,return v before subed
     */
#define spx_atomic_decr(v) __sync_sub_and_fetch(&(v), 1)

/*
     * v + a,return v after added
     */
#define spx_atomic_lazy_add(v, a) __sync_fetch_and_add(&(v), a)

/*
     * v - s,return v after subed
     */
#define spx_atomic_lazy_sub(v, s) __sync_fetch_and_sub(&(v), s)

/*
     * v + 1 ,return v after added
     */
#define spx_atomic_lazy_incr(v) __sync_fetch_and_add(&(v), 1)

/*
     * v - 1,return v after subed
     */
#define spx_atomic_lazy_decr(v) __sync_fetch_and_sub(&(v), 1)

/*
     * set v to 0
     * */
#define spx_atomic_release(v) __sync_lock_release(&(v))

/*
     * set n to v and return v
     * */
#define spx_atomic_tas(v, n) __sync_lock_test_and_set(&(v), n)

/*
     * try set n to v when o == v,return true if set is success
     * */
#define spx_atomic_trycas(v, o, n) __sync_bool_compare_and_swap(&(v), o, n)

/*
     * try set n to v when o == v,return v if set is success
     * */
#define spx_atomic_cas(v, o, n) __sync_val_compare_and_swap(&(v), o, n)

#define spx_atomic_mb( ) __sync_synchronize( )
#define spx_atomic_rmb( ) SpxAtomicMB( )
#define spx_atomic_wmb( ) SpxAtomicMB( )

#ifdef __cplusplus
}
#endif
#endif
