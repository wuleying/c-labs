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

typedef struct luo_dlist_node_s {
    struct luo_dlist_node_s *prev;
    struct luo_dlist_node_s *next;
    void                    *value;
} luo_dlist_node;

typedef struct luo_dlist_iter_s {
    luo_dlist_node *next;
    int            direction;
} luo_dlist_iter;

typedef struct luo_dlist_s {
    luo_dlist_node *head;
    luo_dlist_node *tail;

    void *(*dup)(void *ptr);

    void (*free)(void *ptr);

    int  (*match)(void *ptr, void *key);

    unsigned int   len;
    luo_dlist_iter iter;
} luo_dlist;

/* 常量 */
#define LUO_DLIST_START_HEAD 0
#define LUO_DLIST_START_TAIL 1

/* 宏定义 */
#define LUO_DLIST_LENGTH(luo_dlist) ((luo_dlist)->len)
#define LUO_DLIST_FIRST(luo_dlist) ((luo_dlist)->head)
#define LUO_DLIST_LAST(luo_dlist) ((luo_dlist)->tail)
#define LUO_DLIST_PREV_NODE(luo_dlist_node) ((luo_dlist_node)->prev)
#define LUO_DLIST_NEXT_NODE(luo_dlist_node) ((luo_dlist_node)->next)
#define LUO_DLIST_NODE_VALUE(luo_dlist_node) ((luo_dlist_node)->value)

#define LUO_DLIST_SET_DUP_METHOD(luo_dlist, method) ((luo_dlist)->dup = (method))
#define LUO_DLIST_SET_FREE_METHOD(luo_dlist, method) ((luo_dlist)->free = (method))
#define LUO_DLIST_SET_MATCH_METHOD(luo_dlist, method) ((luo_dlist)->match = (method))

#define LUO_DLIST_GET_DUP_METHOD(luo_dlist) ((luo_dlist)->dup)
#define LUO_DLIST_GET_FREE_METHOD(luo_dlist) ((luo_dlist)->free)
#define LUO_DLIST_GET_MATCH_METHOD(luo_dlist) ((luo_dlist)->match)

/* 接口 */
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
