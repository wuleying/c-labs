//
// _                 _ _             _
// | |_   _  ___   __| | |__   __   _/ |
// | | | | |/ _ \ / _` | '_ \  \ \ / / |
// | | |_| | (_) | (_| | |_) |  \ V /| |
// |_|\__,_|\___/ \__,_|_.__/    \_/ |_|
//
// Created by luoliang1 on 2017/5/4.
//

#include <luodb/structure/dlist.h>

// 创建链表
luo_dlist *
luoDListCreate(void) {
    // 申请内存空间
    luo_dlist *dlist = luoMalloc(sizeof(luo_dlist));

    // 如果申请内存失败直接返回NULL
    if (dlist == NULL) {
        return NULL;
    }

    // 初始化操作
    // 头节点
    dlist->head  = NULL;
    // 尾节点
    dlist->tail  = NULL;
    // 双向链表长度
    dlist->len   = 0;
    // 复制函数指针
    dlist->dup   = NULL;
    // 内存释放函数指针
    dlist->free  = NULL;
    // 匹配函数指针
    dlist->match = NULL;

    return dlist;
}

// 释放整个链表
void
luoDListRelease(luo_dlist *dlist) {
    unsigned int len;

    luo_dlist_node *current;
    luo_dlist_node *next;

    // 获取处理的第一个节点,也就是头节点
    current = dlist->head;
    // 获取链表长度
    len     = dlist->len;

    while (len--) {
        // 当前节点的后继节点
        next = current->next;

        // 如果有自定义内存释放函数,则使用此函数释放节点值
        if (dlist->free) {
            dlist->free(current->value);
        }

        // 释放当前节点
        luoFree(current);
        // 将后继节点设为当前节点,继续释放
        current = next;
    }

    // 释放整个链表
    luoFree(dlist);
}

// 添加头节点
luo_dlist *
luoDListAddNodeHead(luo_dlist *dlist, void *value) {
    // 为节点申请内存空间
    luo_dlist_node *node = luoMalloc(sizeof(luo_dlist_node));

    // 如果申请内存失败直接返回NULL
    if (node == NULL) {
        return NULL;
    }

    // 节点赋值
    node->value = value;

    if (dlist->len == 0) {
        // 当链表中没有任何节点是,节点前驱后继都设为NULL,并将头尾节点都设为当前节点
        node->prev  = NULL;
        node->next  = NULL;
        dlist->head = node;
        dlist->tail = node;
    } else {
        // 当链表中已经存在节点,将原来的链表头节点后移
        node->prev        = NULL;
        // 后继为原头节点
        node->next        = dlist->head;
        // 设置原头节点的前驱节点为新增节点
        dlist->head->prev = node;
        // 设置链表头节点
        dlist->head       = node;
    }

    // 链表长度加一
    dlist->len++;

    return dlist;
}

// 添加尾节点
luo_dlist *
luoDListAddNodeTail(luo_dlist *dlist, void *value) {
    // 为节点申请内存空间
    luo_dlist_node *node = luoMalloc(sizeof(luo_dlist_node));

    // 如果申请内存失败直接返回NULL
    if (node == NULL) {
        return NULL;
    }

    // 节点赋值
    node->value = value;

    if (dlist->len == 0) {
        // 当链表中没有任何节点是,节点前驱后继都设为NULL,并将头尾节点都设为当前节点
        node->prev  = NULL;
        node->next  = NULL;
        dlist->head = node;
        dlist->tail = node;
    } else {
        // 当链表中已经存在节点,将原来的链表尾节点前移
        node->prev        = dlist->tail;
        node->next        = NULL;
        // 原尾节点后继为新增节点
        dlist->tail->next = node;
        // 设置链表尾节点
        dlist->tail       = node;
    }

    // 链表长度加一
    dlist->len++;

    return dlist;
}

// 删除节点
void
luoDListDeleteNode(luo_dlist *dlist, luo_dlist_node *node) {
    if (node->prev) {
        node->prev->next = node->next;
    } else {
        dlist->head = node->next;
    }

    if (node->next) {
        node->next->prev = node->prev;
    } else {
        dlist->tail = node->prev;
    }

    if (dlist->free) {
        dlist->free(node->value);
    }

    luoFree(node);

    dlist->len--;
}

luo_dlist_iter *
luoDListGetIterator(luo_dlist *dlist, int direction) {
    luo_dlist_iter *iter = luoMalloc(sizeof(luo_dlist_iter));

    if (iter == NULL) {
        return NULL;
    }

    iter->next = (direction == LUO_DLIST_START_HEAD) ? dlist->head : dlist->tail;

    iter->direction = direction;

    return iter;
}

void
luoDListReleaseIterator(luo_dlist_iter *iter) {
    luoFree(iter);
}

luo_dlist_node *
luoDListNext(luo_dlist_iter *iter) {
    luo_dlist_node *current = iter->next;

    if (current != NULL) {
        iter->next = (iter->direction == LUO_DLIST_START_HEAD) ? current->next : current->prev;
    }

    return current;
}

luo_dlist *
luoDListDup(luo_dlist *dlist) {
    luo_dlist      *copy;
    luo_dlist_iter *iter;
    luo_dlist_node *node;

    copy = luoDListCreate();

    if (copy == NULL) {
        return NULL;
    }

    copy->dup   = dlist->dup;
    copy->free  = dlist->free;
    copy->match = dlist->match;

    iter = luoDListGetIterator(dlist, LUO_DLIST_START_HEAD);

    while ((node = luoDListNext(iter)) != NULL) {
        void *value;

        if (copy->dup) {
            value = copy->dup(node->value);
            if (value == NULL) {
                luoDListRelease(copy);
                luoDListReleaseIterator(iter);
                return NULL;
            }
        } else {
            value = node->value;
        }

        if (luoDListAddNodeTail(copy, value) == NULL) {
            luoDListRelease(copy);
            luoDListReleaseIterator(iter);
            return NULL;
        }
    }

    luoDListReleaseIterator(iter);

    return copy;
}

luo_dlist_node *
luoDListSearchKey(luo_dlist *dlist, void *key) {
    luo_dlist_iter *iter;
    luo_dlist_node *node;

    iter = luoDListGetIterator(dlist, LUO_DLIST_START_HEAD);

    while ((node = luoDListNext(iter)) != NULL) {
        if (dlist->match) {
            if (dlist->match(node->value, key)) {
                luoDListReleaseIterator(iter);
                return node;
            }
        } else {
            if (key == node->value) {
                luoDListReleaseIterator(iter);
                return node;
            }
        }
    }

    luoDListReleaseIterator(iter);
    return NULL;
}

luo_dlist_node *
luoDListIndex(luo_dlist *dlist, int index) {
    luo_dlist_node *node;

    if (index < 0) {
        index = (-index) - 1;
        node  = dlist->tail;
        while (index-- && node) {
            node = node->prev;
        }
    } else {
        node = dlist->head;
        while (index-- && node) {
            node = node->next;
        }
    }

    return node;
}

void
luoDListRewind(luo_dlist *dlist) {
    dlist->iter.next      = dlist->head;
    dlist->iter.direction = LUO_DLIST_START_HEAD;
}

void
luoDListRewindTail(luo_dlist *dlist) {
    dlist->iter.next      = dlist->tail;
    dlist->iter.direction = LUO_DLIST_START_TAIL;
}

luo_dlist_node *
luoDListYield(luo_dlist *dlist) {
    return luoDListNext(&dlist->iter);
}