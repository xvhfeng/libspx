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
 *       Filename:  spx_khlist.h
 *        Created:  2016/09/21 19时19分03秒
 *         Author:  Seapeak.Xu (www.94geek.com), xvhfeng@gmail.com
 *        Company:  Tencent Literature
 *         Remark:
 *
 ****************************************************************************/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>

#include "spx_ctype.h"
#include "spx_mfunc.h"

#include "spx_klist.h"

typedef struct spx_khlist_node {
    struct spx_khlist_node *next, **pprev;
} spx_khlist_node_t;

typedef struct spx_khlist_head {
    struct spx_khlist_node *first;
} spx_khlist_head_t;

/*
     * Double linked lists with a single pointer list head.
     * Mostly useful for hash tables where the two pointer list head is
     * too wasteful.
     * You lose the ability to access the tail in O(1).
     */

#define SPX_KHLIST_HEAD_INIT \
    {                        \
        .first = NULL        \
    }
#define SPX_KHLIST_HEAD(name) spx_khlist_head_t name = { .first = NULL }
#define SPX_KHLIST_INIT_HEAD(ptr) ((ptr)->first = NULL)

#define spx_khlist_define(name, len) \
    spx_hlist_head_t name[ len ] = { [0 ...(len - 1)] = SPX_KHLIST_HEAD_INIT }

#define spx_khlist_declare(name, len) \
    spx_khlist_head_t name[ len ] = { 0 }

__SpxPrivate__ __SpxInlint__ void
spx_khlist_init(spx_khlist_head_t *ht,
                const i32_t len) {
    i32_t i = 0;
    for (; i < len; i++) {
        SPX_KHLIST_INIT_HEAD(&(ht[i]);
    }
}

__SpxPrivate__ __SpxInline__ void
spx_khlist_init_node(spx_khlist_node_t *h) {
    h->next = NULL;
    h->pprev = NULL;
}

__SpxPrivate__ __SpxInline__ int
spx_khlist_unhashed(const spx_khlist_node_t *h) {
    return !h->pprev;
}

__SpxPrivate__ __SpxInline__ int
spx_khlist_empty(const spx_khlist_node_t *h) {
    return !(h->first);
}

__SpxPrivate__ __SpxInline__ void
__spx_khlist_del(spx_khlist_node_t *n) {
    spx_khlist_node_t *next = n->next;
    spx_khlist_node_t **pprev = n->pprev;

    *pprev = next;
    if (next)
        next->pprev = pprev;
}

__SpxPrivate__ __SpxInline__ void
spx_khlist_del(spx_khlist_node_t *n) {
    __spx_khlist_del(n);
    n->next = SPX_KLIST_POISON1;
    n->pprev = SPX_KLIST_POISON2;
}

__SpxPrivate__ __SpxInline__ void
spx_khlist_del_init(spx_khlist_node_t *n) {
    if (!spx_khlist_unhashed(n)) {
        __spx_khlist_del(n);
        spx_khlist_init_node(n);
    }
}

__SpxPrivate__ __SpxInline__ void
spx_khlist_add_head(spx_khlist_node_t *n,
                    spx_khlist_node_t *h) {
    spx_khlist_node_t *first = h->first;
    n->next = first;
    if (first)
        first->pprev = &n->next;
    h->first = n;
    n->pprev = &h->first;
}

/* next must be != NULL */
__SpxPrivate__ __SpxInline__ void
spx_khlist_add_before(spx_khlist_node_t *n,
                      spx_khlist_node_t *next) {
    n->pprev = next->pprev;
    n->next = next;
    next->pprev = &n->next;
    *(n->pprev) = n;
}

__SpxPrivate__ __SpxInline__ void
spx_khlist_add_behind(spx_khlist_node_t *n,
                      spx_khlist_node_t *prev) {
    n->next = prev->next;
    prev->next = n;
    n->pprev = &prev->next;

    if (n->next)
        n->next->pprev = &n->next;
}

/* after that we'll appear to be on some hlist and spx_khlist_del will work */
__SpxPrivate__ __SpxInline__ void
spx_khlist_add_fake(spx_khlist_node_t *n) {
    n->pprev = &n->next;
}

__SpxPrivate__ __SpxInline__ bool_t
spx_khlist_fake(spx_khlist_node_t *h) {
    return h->pprev == &h->next;
}

/*
     * Check whether the node is the only node of the head without
     * accessing head:
     */
__SpxPrivate__ __SpxInline__ bool_t
spx_khlist_is_singular_node(spx_khlist_node_t *n,
                            spx_khlist_node_t *h) {
    return !n->next && n->pprev == &h->first;
}

/*
     * Move a list from one list head to another. Fixup the pprev
     * reference of the first entry if it exists.
     */
__SpxPrivate__ __SpxInline__ void
spx_khlist_move_list(spx_khlist_node_t *old,
                     spx_khlist_node_t *new) {
    new->first = old->first;
    if (new->first)
        new->first->pprev = &new->first;
    old->first = NULL;
}

#define spx_khlist_entry(ptr, type, member) container_of(ptr, type, member)

#define spx_khlist_for_each(pos, head) \
    for (pos = (head)->first; pos; pos = pos->next)

#define spx_khlist_for_each_safe(pos, n, head) \
    for (pos = (head)->first; pos && ({ n = pos->next; 1; });    \
         pos = n)

#define spx_khlist_entry_safe(ptr, type, member)                  \
    ({                                                            \
        typeof(ptr) ____ptr = (ptr);                              \
        ____ptr ? spx_khlist_entry(____ptr, type, member) : NULL; \
    })

    /**
     * spx_khlist_for_each_entry	- iterate over list of given type
     * @pos:	the type * to use as a loop cursor.
     * @head:	the head for your list.
     * @member:	the name of the spx_khlist_node within the struct.
     */
#define spx_khlist_for_each_entry(pos, head, member)                         \
    for (pos = spx_khlist_entry_safe((head)->first, typeof(*(pos)), member); \
         pos;                                                                \
         pos = spx_khlist_entry_safe((pos)->member.next, typeof(*(pos)), member))

    /**
     * spx_khlist_for_each_entry_continue - iterate over a hlist continuing after current point
     * @pos:	the type * to use as a loop cursor.
     * @member:	the name of the spx_khlist_node within the struct.
     */
#define spx_khlist_for_each_entry_continue(pos, member)                           \
    for (pos = spx_khlist_entry_safe((pos)->member.next, typeof(*(pos)), member); \
         pos;                                                                     \
         pos = spx_khlist_entry_safe((pos)->member.next, typeof(*(pos)), member))

    /**
     * spx_khlist_for_each_entry_from - iterate over a hlist continuing from current point
     * @pos:	the type * to use as a loop cursor.
     * @member:	the name of the spx_khlist_node within the struct.
     */
#define spx_khlist_for_each_entry_from(pos, member) \
    for (; pos;                                     \
         pos = spx_khlist_entry_safe((pos)->member.next, typeof(*(pos)), member))

    /**
     * spx_khlist_for_each_entry_safe - iterate over list of given type safe against removal of list entry
     * @pos:	the type * to use as a loop cursor.
     * @n:		another &struct spx_khlist_node to use as temporary storage
     * @head:	the head for your list.
     * @member:	the name of the spx_khlist_node within the struct.
     */
#define spx_khlist_for_each_entry_safe(pos, n, head, member)               \
    for (pos = spx_khlist_entry_safe((head)->first, typeof(*pos), member); \
         pos && ({ n = pos->member.next; 1; });                                                     \
         pos = spx_khlist_entry_safe(n, typeof(*pos), member))

#ifdef __cplusplus
}
#endif
