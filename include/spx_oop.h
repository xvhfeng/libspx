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
 *       Filename:  spx_oop.h
 *        Created:  2018/08/06 11时08分35秒
 *         Author:  Seapeak.Xu (www.94geek.com), xvhfeng@gmail.com
 *        Company:  Tencent Literature
 *         Remark:
 *
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#ifndef _SPX_OOP_H_
#define _SPX_OOP_H_
#ifdef __cplusplus
extern "C" {
#endif

#define C_copy(to, from) to->copy(to, from)

#define C_OO_PROTOTYPE(type)                                                                 \
    void type##_init(struct type##_struct *my);                                              \
    void type##_dispose(struct type##_struct *my);                                           \
    char type##_equal(struct type##_struct *my, struct type##_struct *yours);                \
    struct type##_struct *type##_copy(struct type##_struct *my, struct type##_struct *from); \
    const type type##__prototype = { type##_init, type##_dispose, type##_equal, type##_copy

#define C_OO_OVERHEAD(type)                                                 \
    void (*init)(struct type##_struct * my);                                \
    void (*dispose)(struct type##_struct * my);                             \
    char (*equal)(struct type##_struct * my, struct type##_struct * yours); \
    struct type##_struct *(*copy)(struct type##_struct * my, struct type##_struct * from);

#define C_OO_IN(ret, type, function, ...) ret (*function)(struct type##_struct * my, __VA_ARGS__);

#define C_OO_OUT(ret, type, function, ...) ret type##_##function(struct type##_struct *my, __VA_ARGS__);

#define C_OO_PNEW(type, instance)             \
    instance = (type *) malloc(sizeof(type)); \
    memcpy(instance, &type##__prototype, sizeof(type));

#define C_OO_NEW(type, instance) \
    type instance;               \
    memcpy(&instance, &type##__prototype, sizeof(type));

#define C_OO_DELETE(instance)    \
    instance->dispose(instance); \
    free(instance);

#define C_OO_INIT(type)                          \
    void type##_init(struct type##_struct *my) { \
        return;                                  \
    }

#define C_OO_DISPOSE(type)                          \
    void type##_dispose(struct type##_struct *my) { \
        return;                                     \
    }

#define C_OO_EQUAL(type)                                                       \
    char type##_equal(struct type##_struct *my, struct type##_struct *yours) { \
        return 0;                                                              \
    }

#define C_OO_COPY(type)                                                                       \
    struct type##_struct *type##_copy(struct type##_struct *my, struct type##_struct *from) { \
        return 0;                                                                             \
    }

#ifdef __cplusplus
}
#endif
#endif
