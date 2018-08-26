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
 *       Filename:  spx_list.h
 *        Created:  2018/08/03 14时16分19秒
 *         Author:  Seapeak.Xu (www.94geek.com), xvhfeng@gmail.com
 *        Company:  Tencent Literature
 *         Remark:
 *
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#ifndef __Spx_CHAIN__
#define __Spx_CHAIN__
#ifdef __cplusplus
extern "C" {
#endif

#include "spx_ctype.h"
#include "spx_klist.h"

typedef struct {
    spx_klist_node_t head;
    sint_t count;
} spx_chain_t;

__SpxPrivate__ __SpxInline__ void spx_chain_init(spx_chain_t *chain) {
    spx_klist_init_head(&(chain->head));
}

__SpxPrivate__ __SpxInline__ void spx_chain_add(spx_chain_t *chain, spx_klist_node_t *new) {
    spx_klist_add(new, &(chain->head));
    spx_atomic_add(chain->count);
}

__SpxPrivate__ __SpxInline__ void spx_chain_add_tail(spx_chain_t *chain, spx_klist_node_t *new) {
    spx_klist_add_tail(new, &(chain->head));
    spx_atomic_incr(chain->count);
}

__SpxPrivate__ __SpxInline__ void spx_chain_del(spx_chain_t *chain, spx_klist_node_t *entry) {
    spx_klist_del(entry);
    spx_atomic_decr(chain->count);
}

__SpxPrivate__ __SpxInline__ void spx_chain_del_entry(spx_chain_t *chain, spx_klist_node_t *entry) {
    spx_klist_del_entry(entry);
    spx_atomic_decr(chain->count);
}

__SpxPrivate__ __SpxInline__ spx_klist_node_t *spx_chain_del_tail(spx_chain_t *chain) {
    spx_atomic_decr(chain->count);
    return spx_klist_del_tail(&(chain->head));
}

__SpxPrivate__ __SpxInline__ spx_klist_node_t *spx_chain_del_head(spx_chain_t *chain) {
    spx_atomic_decr(chain->count);
    return spx_klisf_del_head(&(chain->head));
}
#ifdef __cplusplus
}
#endif
#endif
