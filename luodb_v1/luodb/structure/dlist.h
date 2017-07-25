//
// _                 _ _             _
// | |_   _  ___   __| | |__   __   _/ |
// | | | | |/ _ \ / _` | '_ \  \ \ / / |
// | | |_| | (_) | (_| | |_) |  \ V /| |
// |_|\__,_|\___/ \__,_|_.__/    \_/ |_|
//
// Created by luoliang1 on 2017/5/4.
//

#ifndef LUODB_DLIST_H
#define LUODB_DLIST_H

#include <luodb/memory/mem.h>

// 链表节点
typedef struct luo_dlist_node_s {
    // 前驱节点
    struct luo_dlist_node_s *prev;
    // 后继节点
    struct luo_dlist_node_s *next;
    // 节点值指针
    void                    *value;
} luo_dlist_node;

// 链表迭代器 只能单向迭代
typedef struct luo_dlist_iter_s {
    // 当前迭代节点的下一节点
    luo_dlist_node *next;
    // 迭代器方向
    int            direction;
} luo_dlist_iter;

// 双向链表
typedef struct luo_dlist_s {
    // 头节点
    luo_dlist_node *head;
    // 尾节点
    luo_dlist_node *tail;

    // 复制函数指针
    void *(*dup)(void *ptr);

    // 释放内存函数指针
    void (*free)(void *ptr);

    // 匹配函数指针
    int  (*match)(void *ptr, void *key);

    // 双向链表长度
    unsigned int   len;
    // 双向链表迭代器
    luo_dlist_iter iter;
} luo_dlist;

/* 常量 */
// 迭代方向 从头向尾迭代
#define LUO_DLIST_START_HEAD 0
// 迭代方向 从尾向头迭代
#define LUO_DLIST_START_TAIL 1

/* 宏定义 */
// 获取链表长度
#define LUO_DLIST_LENGTH(luo_dlist) ((luo_dlist)->len)
// 获取链表头部节点
#define LUO_DLIST_FIRST(luo_dlist) ((luo_dlist)->head)
// 获取链表尾部节点
#define LUO_DLIST_LAST(luo_dlist) ((luo_dlist)->tail)
// 获取给定节点的前驱节点
#define LUO_DLIST_PREV_NODE(luo_dlist_node) ((luo_dlist_node)->prev)
// 获取给定节点的后继节点
#define LUO_DLIST_NEXT_NODE(luo_dlist_node) ((luo_dlist_node)->next)
// 获取给定节点的值 这里是一个指针
#define LUO_DLIST_NODE_VALUE(luo_dlist_node) ((luo_dlist_node)->value)

// 设置链表的复制方法
#define LUO_DLIST_SET_DUP_METHOD(luo_dlist, method) ((luo_dlist)->dup = (method))
// 设置链表的内存释放方法
#define LUO_DLIST_SET_FREE_METHOD(luo_dlist, method) ((luo_dlist)->free = (method))
// 设置链表的匹配方法
#define LUO_DLIST_SET_MATCH_METHOD(luo_dlist, method) ((luo_dlist)->match = (method))

// 获取链表的复制方法
#define LUO_DLIST_GET_DUP_METHOD(luo_dlist) ((luo_dlist)->dup)
// 获取链表的内存释放方法
#define LUO_DLIST_GET_FREE_METHOD(luo_dlist) ((luo_dlist)->free
// 获取链表的内存释放方法
#define LUO_DLIST_GET_MATCH_METHOD(luo_dlist) ((luo_dlist)->match)

/* 接口定义 */
luo_dlist *luoDListCreate(void);

void luoDListRelease(luo_dlist *dlist);

luo_dlist *luoDListAddNodeHead(luo_dlist *dlist, void *value);

luo_dlist *luoDListAddNodeTail(luo_dlist *dlist, void *value);

void luoDListDeleteNode(luo_dlist *dlist, luo_dlist_node *node);

luo_dlist_iter *luoDListGetIterator(luo_dlist *dlist, int direction);

void luoDListReleaseIterator(luo_dlist_iter *iter);

luo_dlist_node *luoDListNext(luo_dlist_iter *iter);

luo_dlist *luoDListDup(luo_dlist *dlist);

luo_dlist_node *luoDListSearchKey(luo_dlist *dlist, void *key);

luo_dlist_node *luoDListIndex(luo_dlist *dlist, int index);

void luoDListRewind(luo_dlist *dlist);

void luoDListRewindTail(luo_dlist *dlist);

luo_dlist_node *luoDListYield(luo_dlist *dlist);

#endif //LUODB_DLIST_H
