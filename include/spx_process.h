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
 *       Filename:  spx_process.h
 *        Created:  2018年08月10日 01时06分04秒
 *         Author:  Seapeak.Xu (www.94geek.com), xvhfeng@gmail.com
 *        Company:  Tencent Literature
 *         Remark:
 *
 ****************************************************************************/

#ifndef _SPX_PROCESS_H_
#define _SPX_PROCESS_H_
#ifdef __cplusplus
extern "C" {
#endif
#define __USE_GNU
#include <stdio.h>
#include <stdlib.h>

#include <sys/sysinfo.h>
#include <unistd.h>

#include <ctype.h>
#include <pthread.h>
#include <sched.h>
#include <string.h>

#include "spx_ctype.h"

typedef void(spx_worker_process_main_fn)(void);

#if (_POSIX_PRIORITY_SCHEDULING)
#define spx_sched_yield( ) sched_yield( )
#else
#define spx_sched_yield( ) spin_delay( )
#endif

/*
     * get all cpus include offline cpus
     */
__SpxPrivate__ __SpxInline__ i32_t
spx_sched_all_cpus(void) { /*{{{*/
    i32_t cpus = 0;
#if _SC_NPROCESSORS_CONF
    cpus = sysconf(_SC_NPROCESSORS_CONF);
#endif
    if (0 != cpus)
        return cpus;
    return get_nprocs_conf( );
} /*}}}*/

/*
     *  get all available cpus ignore offline cpus
     */
__SpxPrivate__ __SpxInline__ i32_t
spx_sched_avail_cpus(void) { /*{{{*/
    i32_t cpus = 0;
#if _SC_NPROCESSORS_ONLN
    cpus = sysconf(_SC_NPROCESSORS_ONLN);
#endif
    if (0 != cpus)
        return cpus;
    return get_nprocs( );
} /*}}}*/

/*
     * bind current thread/process to cpu
     */
__SpxPrivate__ __SpxInline__ spx_ret_t
spx_sched_bind_curr_thread_to_cpu(i32_t cpu_idx) { /*{{{*/
    spx_ret_t ret = SpxRet_OK;
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(cpu_idx, &mask);
    spx_errno_clear;
    if (-1 == sched_setaffinity(0, sizeof(mask), &mask)) {
        ret = 0 == errno ? SpxRet_BindCpuFail : errno;
    }
    return ret;
} /*}}}*/

/*
 * delay cpu
 * */
__SpxPrivate__ __SpxInline__ void
spin_delay(void) { /*{{{*/
    __asm__ __volatile__(
        " rep; nop       \n");
} /*}}}*/

#ifdef __cplusplus
}
#endif
#endif
