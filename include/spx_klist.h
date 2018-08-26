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
 *       Filename:  spx_klist.h
 *        Created:  2018/06/25 19时19分03秒
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

#include "spx_atomic.h"
#include "spx_ctype.h"
#include "spx_mfunc.h"
#include "spx_ret.h"

typedef struct spx_klist_node {
    struct spx_klist_node *prev;
    struct spx_klist_node *next;
} spx_klist_node_t;

#ifdef SPX_KLIST_POISON1
#undef SPX_KLIST_POISON1
#endif
#define SPX_KLIST_POISON1 NULL

#ifdef SPX_KLIST_POISON2
#undef SPX_KLIST_POISON2
#endif
#define SPX_KLIST_POISON2 NULL

/*
	 * Simple doubly linked list implementation.
	 *
	 * Some of the internal functions ("__xxx") are useful when
	 * manipulating whole lists rather than single entries, as
	 * sometimes we already know the next/prev entries and we can
	 * generate better code by using them directly rather than
	 * using the generic single-entry routines.
	 */
#define SPX_KLIST_HEAD_INIT(name) \
    { &(name), &(name) }

#define SPX_KLIST_HEAD(name) \
    spx_klist_node_t name = SPX_KLIST_HEAD_INIT(name)

__SpxPrivate__ __SpxInline__ void spx_klist_init_head(spx_klist_node_t *list) {
    list->next = list;
    list->prev = list;
}

/*
	 * Insert a new entry between two known consecutive entries.
	 *
	 * This is only for internal list manipulation where we know
	 * the prev/next entries already!
	 */
__SpxPrivate__ __SpxInline__ void __spx_klist_add(spx_klist_node_t *new,
                                                  spx_klist_node_t *prev,
                                                  spx_klist_node_t *next) {
    next->prev = new;
    new->next = next;
    new->prev = prev;
    prev->next = new;
}

/**
	 * spx_klist_add - add a new entry
	 * @new: new entry to be added
	 * @head: list head to add it after
	 *
	 * Insert a new entry after the specified head.
	 * This is good for implementing stacks.
	 */
__SpxPrivate__ __SpxInline__ void spx_klist_add(spx_klist_node_t *new, spx_klist_node_t *head) {
    __spx_klist_add(new, head, head->next);
}

/**
	 * spx_klist_add_tail - add a new entry
	 * @new: new entry to be added
	 * @head: list head to add it before
	 *
	 * Insert a new entry before the specified head.
	 * This is useful for implementing queues.
	 */
__SpxPrivate__ __SpxInline__ void spx_klist_add_tail(spx_klist_node_t *new, spx_klist_node_t *head) {
    __spx_klist_add(new, head->prev, head);
}

/*
	 * Delete a list entry by making the prev/next entries
	 * point to each other.
	 *
	 * This is only for internal list manipulation where we know
	 * the prev/next entries already!
	 */
__SpxPrivate__ __SpxInline__ void __spx_klist_del(spx_klist_node_t *prev, spx_klist_node_t *next) {
    next->prev = prev;
    prev->next = next;
}

/**
	 * spx_klist_del - deletes entry from list.
	 * @entry: the element to delete from the list.
	 * Note: spx_klist_is_empty() on entry does not return true after this, the entry is
	 * in an undefined state.
	 */
__SpxPrivate__ __SpxInline__ void __spx_klist_del_entry(spx_klist_node_t *entry) {
    __spx_klist_del(entry->prev, entry->next);
}

__SpxPrivate__ __SpxInline__ void spx_klist_del(spx_klist_node_t *entry) {
    __spx_klist_del(entry->prev, entry->next);
    entry->next = SPX_KLIST_POISON1;
    entry->prev = SPX_KLIST_POISON2;
}

__SpxPrivate__ __SpxInline__ spx_klist_node_t *spx_klist_del_tail(spx_klist_node_t *head) {
    spx_klist_node_t *entry = head->prev;
    __spx_klist_del_entry(entry);
    return entry;
}

__SpxPrivate__ __SpxInline__ spx_klist_node_t *spx_klist_del_head(spx_klist_node_t *head) {
    spx_klist_node_t *entry = head->next;
    __spx_klist_del_entry(entry);
    return entry;
}

/**
	 * spx_klist_replace - replace old entry by new one
	 * @old : the element to be replaced
	 * @new : the new element to insert
	 *
	 * If @old was empty, it will be overwritten.
	 */
__SpxPrivate__ __SpxInline__ void spx_klist_replace(spx_klist_node_t *old,
                                                    spx_klist_node_t *new) {
    new->next = old->next;
    new->next->prev = new;
    new->prev = old->prev;
    new->prev->next = new;
}

__SpxPrivate__ __SpxInline__ void spx_klist_replace_init(spx_klist_node_t *old,
                                                         spx_klist_node_t *new) {
    spx_klist_replace(old, new);
    spx_klist_init_head(old);
}

/**
	 * spx_klist_del_init - deletes entry from list and reinitialize it.
	 * @entry: the element to delete from the list.
	 */
__SpxPrivate__ __SpxInline__ void spx_klist_del_init(spx_klist_node_t *entry) {
    __spx_klist_del_entry(entry);
    spx_klist_init_head(entry);
}

/**
	 * spx_klist_move - delete from one list and add as another's head
	 * @list: the entry to move
	 * @head: the head that will precede our entry
	 */
__SpxPrivate__ __SpxInline__ void spx_klist_move(spx_klist_node_t *list, spx_klist_node_t *head) {
    __spx_klist_del_entry(list);
    spx_klist_add(list, head);
}

/**
	 * spx_klist_move_tail - delete from one list and add as another's tail
	 * @list: the entry to move
	 * @head: the head that will follow our entry
	 */
__SpxPrivate__ __SpxInline__ void spx_klist_move_tail(spx_klist_node_t *list,
                                                      spx_klist_node_t *head) {
    __spx_klist_del_entry(list);
    spx_klist_add_tail(list, head);
}

/**
	 * spx_klist_is_last - tests whether @list is the last entry in list @head
	 * @list: the entry to test
	 * @head: the head of the list
	 */
__SpxPrivate__ __SpxInline__ int spx_klist_is_last(const spx_klist_node_t *list,
                                                   const spx_klist_node_t *head) {
    return list->next == head;
}

/**
	 * spx_klist_is_empty - tests whether a list is empty
	 * @head: the list to test.
	 */
__SpxPrivate__ __SpxInline__ int spx_klist_is_empty(const spx_klist_node_t *head) {
    return head->next == head;
}

/**
	 * spx_klist_is_empty_careful - tests whether a list is empty and not being modified
	 * @head: the list to test
	 *
	 * Description:
	 * tests whether a list is empty _and_ checks that no other CPU might be
	 * in the process of modifying either member (next or prev)
	 *
	 * NOTE: using spx_klist_is_empty_careful() without synchronization
	 * can only be safe if the only activity that can happen
	 * to the list entry is spx_klist_del_init(). Eg. it cannot be used
	 * if another CPU could re-spx_klist_add() it.
	 */
__SpxPrivate__ __SpxInline__ int spx_klist_is_empty_careful(const spx_klist_node_t *head) {
    spx_klist_node_t *next = head->next;
    return (next == head) && (next == head->prev);
}

/**
	 * spx_klist_rotate_left - rotate the list to the left
	 * @head: the head of the list
	 */
__SpxPrivate__ __SpxInline__ void spx_klist_rotate_left(spx_klist_node_t *head) {
    spx_klist_node_t *first;

    if (!spx_klist_is_empty(head)) {
        first = head->next;
        spx_klist_move_tail(first, head);
    }
}

/**
	 * spx_klist_is_singular - tests whether a list has just one entry.
	 * @head: the list to test.
	 */
__SpxPrivate__ __SpxInline__ int spx_klist_is_singular(const spx_klist_node_t *head) {
    return !spx_klist_is_empty(head) && (head->next == head->prev);
}

__SpxPrivate__ __SpxInline__ void __spx_klist_cut_position(spx_klist_node_t *list,
                                                           spx_klist_node_t *head,
                                                           spx_klist_node_t *entry) {
    spx_klist_node_t *new_first = entry->next;
    list->next = head->next;
    list->next->prev = list;
    list->prev = entry;
    entry->next = list;
    head->next = new_first;
    new_first->prev = head;
}

/**
	 * spx_klist_cut_position - cut a list into two
	 * @list: a new list to add all removed entries
	 * @head: a list with entries
	 * @entry: an entry within head, could be the head itself
	 *	and if so we won't cut the list
	 *
	 * This helper moves the initial part of @head, up to and
	 * including @entry, from @head to @list. You should
	 * pass on @entry an element you know is on @head. @list
	 * should be an empty list or a list you do not care about
	 * losing its data.
	 *
	 */
__SpxPrivate__ __SpxInline__ void spx_klist_cut_position(spx_klist_node_t *list,
                                                         spx_klist_node_t *head,
                                                         spx_klist_node_t *entry) {
    if (spx_klist_is_empty(head))
        return;
    if (spx_klist_is_singular(head) && (head->next != entry && head != entry))
        return;
    if (entry == head)
        spx_klist_init_head(list);
    else
        __spx_klist_cut_position(list, head, entry);
}

__SpxPrivate__ __SpxInline__ void spx_klist_clear(spx_klist_node_t *head, spx_klist_node_t *list) {
    if (spx_klist_is_empty(head))
        return;
    spx_klist_cut_position(list, head, head->next);
}

__SpxPrivate__ __SpxInline__ void __spx_klist_splice(const spx_klist_node_t *list,
                                                     spx_klist_node_t *prev,
                                                     spx_klist_node_t *next) {
    spx_klist_node_t *first = list->next;
    spx_klist_node_t *last = list->prev;

    first->prev = prev;
    prev->next = first;

    last->next = next;
    next->prev = last;
}

/**
	 * spx_klist_splice - join two lists, this is designed for stacks
	 * @list: the new list to add.
	 * @head: the place to add it in the first list.
	 */
__SpxPrivate__ __SpxInline__ void spx_klist_splice(const spx_klist_node_t *list,
                                                   spx_klist_node_t *head) {
    if (!spx_klist_is_empty(list))
        __spx_klist_splice(list, head, head->next);
}

/**
	 * spx_klist_splice_tail - join two lists, each list being a queue
	 * @list: the new list to add.
	 * @head: the place to add it in the first list.
	 */
__SpxPrivate__ __SpxInline__ void spx_klist_splice_tail(spx_klist_node_t *list,
                                                        spx_klist_node_t *head) {
    if (!spx_klist_is_empty(list))
        __spx_klist_splice(list, head->prev, head);
}

/**
	 * spx_klist_splice_init - join two lists and reinitialise the emptied list.
	 * @list: the new list to add.
	 * @head: the place to add it in the first list.
	 *
	 * The list at @list is reinitialised
	 */
__SpxPrivate__ __SpxInline__ void spx_klist_splice_init(spx_klist_node_t *list,
                                                        spx_klist_node_t *head) {
    if (!spx_klist_is_empty(list)) {
        __spx_klist_splice(list, head, head->next);
        spx_klist_init_head(list);
    }
}

/**
	 * spx_klist_splice_tail_init - join two lists and reinitialise the emptied list
	 * @list: the new list to add.
	 * @head: the place to add it in the first list.
	 *
	 * Each of the lists is a queue.
	 * The list at @list is reinitialised
	 */
__SpxPrivate__ __SpxInline__ void spx_klist_splice_tail_init(spx_klist_node_t *list,
                                                             spx_klist_node_t *head) {
    if (!spx_klist_is_empty(list)) {
        __spx_klist_splice(list, head->prev, head);
        spx_klist_init_head(list);
    }
}

/**
	 * spx_klist_entry - get the struct for this entry
	 * @ptr:	the &spx_klist_node_t pointer.
	 * @type:	the type of the struct this is embedded in.
	 * @member:	the name of the spx_klist_node within the struct.
	 */
#define spx_klist_entry(ptr, type, member) \
    spx_container_of(ptr, type, member)

/**
	 * spx_klist_first_entry - get the first element from a list
	 * @ptr:	the list head to take the element from.
	 * @type:	the type of the struct this is embedded in.
	 * @member:	the name of the spx_klist_node within the struct.
	 *
	 * Note, that list is expected to be not empty.
	 */
#define spx_klist_first_entry(ptr, type, member) \
    spx_klist_entry((ptr)->next, type, member)

#define spx_klist_del_tail_entry(head, type, member) \
    spx_klist_entry(spx_klist_del_tail(head), type, member)

#define spx_klist_del_head_entry(head, type, member) \
    spx_klist_entry(spx_klist_del_head(head), type, member)

/**
	 * spx_klist_last_entry - get the last element from a list
	 * @ptr:	the list head to take the element from.
	 * @type:	the type of the struct this is embedded in.
	 * @member:	the name of the spx_klist_node within the struct.
	 *
	 * Note, that list is expected to be not empty.
	 */
#define spx_klist_last_entry(ptr, type, member) \
    spx_klist_entry((ptr)->prev, type, member)

/**
	 * spx_klist_first_entry_or_null - get the first element from a list
	 * @ptr:	the list head to take the element from.
	 * @type:	the type of the struct this is embedded in.
	 * @member:	the name of the spx_klist_node within the struct.
	 *
	 * Note that if the list is empty, it returns NULL.
	 */
#define spx_klist_first_entry_or_null(ptr, type, member) \
    (!spx_klist_is_empty(ptr) ? spx_klist_first_entry(ptr, type, member) : NULL)

/**
	 * spx_klist_next_entry - get the next element in list
	 * @pos:	the type * to cursor
	 * @member:	the name of the spx_klist_node within the struct.
	 */
#define spx_klist_next_entry(pos, type, member) \
    spx_klist_entry((pos)->member.next, type, member)

/**
	 * spx_klist_prev_entry - get the prev element in list
	 * @pos:	the type * to cursor
	 * @member:	the name of the spx_klist_node within the struct.
	 */
#define spx_klist_prev_entry(pos, type, member) \
    spx_klist_entry((pos)->member.prev, type, member)

/**
	 * spx_klist_for_each	-	iterate over a list
	 * @pos:	the &spx_klist_node_t to use as a loop cursor.
	 * @head:	the head for your list.
	 */
#define spx_klist_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

/**
	 * spx_klist_for_each_prev	-	iterate over a list backwards
	 * @pos:	the &spx_klist_node_t to use as a loop cursor.
	 * @head:	the head for your list.
	 */
#define spx_klist_for_each_prev(pos, head) \
    for (pos = (head)->prev; pos != (head); pos = pos->prev)

/**
	 * spx_klist_for_each_safe - iterate over a list safe against removal of list entry
	 * @pos:	the &spx_klist_node_t to use as a loop cursor.
	 * @n:		another &spx_klist_node_t to use as temporary storage
	 * @head:	the head for your list.
	 */
#define spx_klist_for_each_safe(pos, n, head)              \
    for (pos = (head)->next, n = pos->next; pos != (head); \
         pos = n, n = pos->next)

/**
	 * spx_klist_for_each_prev_safe - iterate over a list backwards safe against removal of list entry
	 * @pos:	the &spx_klist_node_t to use as a loop cursor.
	 * @n:		another &spx_klist_node_t to use as temporary storage
	 * @head:	the head for your list.
	 */
#define spx_klist_for_each_prev_safe(pos, n, head) \
    for (pos = (head)->prev, n = pos->prev;        \
         pos != (head);                            \
         pos = n, n = pos->prev)

/**
	 * spx_klist_for_each_entry	-	iterate over list of given type
	 * @pos:	the type * to use as a loop cursor.
	 * @head:	the head for your list.
	 * @member:	the name of the spx_klist_node within the struct.
	 */
#define spx_klist_for_each_entry(pos, type, head, member) \
    for (pos = spx_klist_first_entry(head, type, member); \
         &pos->member != (head);                          \
         pos = spx_klist_next_entry(pos, type, member))

/**
	 * spx_klist_for_each_entry_reverse - iterate backwards over list of given type.
	 * @pos:	the type * to use as a loop cursor.
	 * @head:	the head for your list.
	 * @member:	the name of the spx_klist_node within the struct.
	 */
#define spx_klist_for_each_entry_reverse(pos, type, head, member) \
    for (pos = spx_klist_last_entry(head, type, member);          \
         &pos->member != (head);                                  \
         pos = spx_klist_prev_entry(pos, type, member))

/**
	 * spx_klist_prepare_entry - prepare a pos entry for use in list_for_each_entry_continue()
	 * @pos:	the type * to use as a start point
	 * @head:	the head of the list
	 * @member:	the name of the spx_klist_node within the struct.
	 *
	 * Prepares a pos entry for use as a start point in list_for_each_entry_continue().
	 */
#define spx_klist_prepare_entry(pos, type, head, member) \
    ((pos) ?: spx_klist_entry(head, type, member))

/**
	 * spx_klist_for_each_entry_continue - continue iteration over list of given type
	 * @pos:	the type * to use as a loop cursor.
	 * @head:	the head for your list.
	 * @member:	the name of the spx_klist_node within the struct.
	 *
	 * Continue to iterate over list of given type, continuing after
	 * the current position.
	 */
#define spx_klist_for_each_entry_continue(pos, type, head, member) \
    for (pos = spx_klist_next_entry(pos, type, member);            \
         &pos->member != (head);                                   \
         pos = spx_klist_next_entry(pos, type, member))

/**
	 * spx_klist_for_each_entry_continue_reverse - iterate backwards from the given point
	 * @pos:	the type * to use as a loop cursor.
	 * @head:	the head for your list.
	 * @member:	the name of the spx_klist_node within the struct.
	 *
	 * Start to iterate over list of given type backwards, continuing after
	 * the current position.
	 */
#define spx_klist_for_each_entry_continue_reverse(pos, type, head, member) \
    for (pos = spx_klist_prev_entry(pos, type, member);                    \
         &pos->member != (head);                                           \
         pos = spx_klist_prev_entry(pos, type, member))

/**
	 * spx_klist_for_each_entry_from - iterate over list of given type from the current point
	 * @pos:	the type * to use as a loop cursor.
	 * @head:	the head for your list.
	 * @member:	the name of the spx_klist_node within the struct.
	 *
	 * Iterate over list of given type, continuing from current position.
	 */
#define spx_klist_for_each_entry_from(pos, type, head, member) \
    for (; &pos->member != (head);                             \
         pos = spx_klist_next_entry(pos, type, member))

/**
	 * spx_klist_for_each_entry_safe - iterate over list of given type safe against removal of list entry
	 * @pos:	the type * to use as a loop cursor.
	 * @n:		another type * to use as temporary storage
	 * @head:	the head for your list.
	 * @member:	the name of the spx_klist_node within the struct.
	 */
#define spx_klist_for_each_entry_safe(pos, n, type, head, member) \
    for (pos = spx_klist_first_entry(head, type, member),         \
        n = spx_klist_next_entry(pos, type, member);              \
         &pos->member != (head);                                  \
         pos = n, n = spx_klist_next_entry(n, type, member))

/**
	 * spx_klist_for_each_entry_safe_continue - continue list iteration safe against removal
	 * @pos:	the type * to use as a loop cursor.
	 * @n:		another type * to use as temporary storage
	 * @head:	the head for your list.
	 * @member:	the name of the spx_klist_node within the struct.
	 *
	 * Iterate over list of given type, continuing after current point,
	 * safe against removal of list entry.
	 */
#define spx_klist_for_each_entry_safe_continue(pos, n, type, head, member) \
    for (pos = spx_klist_next_entry(pos, type, member),                    \
        n = spx_klist_next_entry(pos, type, member);                       \
         &pos->member != (head);                                           \
         pos = n, n = spx_klist_next_entry(n, type, member))

/**
	 * spx_klist_for_each_entry_safe_from - iterate over list from current point safe against removal
	 * @pos:	the type * to use as a loop cursor.
	 * @n:		another type * to use as temporary storage
	 * @head:	the head for your list.
	 * @member:	the name of the spx_klist_node within the struct.
	 *
	 * Iterate over list of given type from current point, safe against
	 * removal of list entry.
	 */
#define spx_klist_for_each_entry_safe_from(pos, n, type, head, member) \
    for (n = spx_klist_next_entry(pos, type, member);                  \
         &pos->member != (head);                                       \
         pos = n, n = spx_klist_next_entry(n, type, member))

/**
	 * spx_klist_for_each_entry_safe_reverse - iterate backwards over list safe against removal
	 * @pos:	the type * to use as a loop cursor.
	 * @n:		another type * to use as temporary storage
	 * @head:	the head for your list.
	 * @member:	the name of the spx_klist_node within the struct.
	 *
	 * Iterate backwards over list of given type, safe against removal
	 * of list entry.
	 */
#define spx_klist_for_each_entry_safe_reverse(pos, n, type, head, member) \
    for (pos = spx_klist_last_entry(head, type, member),                  \
        n = spx_klist_prev_entry(pos, type, member);                      \
         &pos->member != (head);                                          \
         pos = n, n = spx_klist_prev_entry(n, type, member))

/**
	 * list_safe_reset_next - reset a stale list_for_each_entry_safe loop
	 * @pos:	the loop cursor used in the list_for_each_entry_safe loop
	 * @n:		temporary storage used in list_for_each_entry_safe
	 * @member:	the name of the spx_klist_node within the struct.
	 *
	 * list_safe_reset_next is not safe to use in general if the list may be
	 * modified concurrently (eg. the lock is dropped in the loop body). An
	 * exception to this is if the cursor element (pos) is pinned in the list,
	 * and list_safe_reset_next is called after re-taking the lock and before
	 * completing the current iteration of the loop body.
	 */
#define spx_klist_safe_reset_next(pos, n, type, member) \
    n = spx_klist_next_entry(pos, type, member)

#ifdef __cplusplus
}
#endif
