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
        // 如果存在前驱节点,将前驱节点的后继修改为当前节点的后继节点
        node->prev->next = node->next;
    } else {
        // 如果不存在前驱节点,说明当前节点为头节点,将链表的头节点修改为当前节点的后继节点
        dlist->head = node->next;
    }

    if (node->next) {
        // 如果存在后继节点,将后继节点的前驱修改为当前节点的前驱节点
        node->next->prev = node->prev;
    } else {
        // 如果不存在后继节点,说明当前节点为尾节点,将链表的尾节点修改为当前节点的前驱节点
        dlist->tail = node->prev;
    }

    if (dlist->free) {
        // 如果定义了内存释放函数,释放当前节点值
        dlist->free(node->value);
    }

    // 释放节点
    luoFree(node);

    // 链表长度减一
    dlist->len--;
}

// 获取链表迭代器
luo_dlist_iter *
luoDListGetIterator(luo_dlist *dlist, int direction) {
    // 为迭代器申请内存空间
    luo_dlist_iter *iter = luoMalloc(sizeof(luo_dlist_iter));

    // 如果申请内存失败直接返回NULL
    if (iter == NULL) {
        return NULL;
    }

    // 根据迭代器迭代方向设置从头节点还是尾节点开始遍历
    iter->next = (direction == LUO_DLIST_START_HEAD) ? dlist->head : dlist->tail;

    // 设置迭代器迭代方向
    iter->direction = direction;

    return iter;
}

// 释放迭代器内存
void
luoDListReleaseIterator(luo_dlist_iter *iter) {
    // 直接释放
    luoFree(iter);
}

// 根据迭代器获取下一节点
luo_dlist_node *
luoDListNext(luo_dlist_iter *iter) {
    // 设置当前节点
    luo_dlist_node *current = iter->next;

    if (current != NULL) {
        // 根据迭代器方向,设置迭代器下一个要处理的节点
        iter->next = (iter->direction == LUO_DLIST_START_HEAD) ? current->next : current->prev;
    }

    return current;
}

// 复制链表
luo_dlist *
luoDListDup(luo_dlist *dlist) {
    luo_dlist      *copy;
    luo_dlist_iter *iter;
    luo_dlist_node *node;

    // 创建一个空链表
    copy = luoDListCreate();

    // 创建失败直接返回NULL
    if (copy == NULL) {
        return NULL;
    }

    // 复制原链表的复制/内存释放/匹配函数指针
    copy->dup   = dlist->dup;
    copy->free  = dlist->free;
    copy->match = dlist->match;

    // 获取链表迭代器
    iter = luoDListGetIterator(dlist, LUO_DLIST_START_HEAD);

    // 根据迭代器遍历节点
    while ((node = luoDListNext(iter)) != NULL) {
        void *value;

        if (copy->dup) {
            // 如果定义了复制方法,使用复制方法复制节点值
            value = copy->dup(node->value);
            if (value == NULL) {
                // 复制失败,释放资源并返回NULL
                luoDListRelease(copy);
                luoDListReleaseIterator(iter);
                return NULL;
            }
        } else {
            // 未定义复制方法,直接复制节点指针
            value = node->value;
        }

        // 从链表尾部添加节点
        if (luoDListAddNodeTail(copy, value) == NULL) {
            // 添加失败,释放资源并返回NULL
            luoDListRelease(copy);
            luoDListReleaseIterator(iter);
            return NULL;
        }
    }

    // 释放迭代器
    luoDListReleaseIterator(iter);

    return copy;
}

// 根据key搜索链表节点
luo_dlist_node *
luoDListSearchKey(luo_dlist *dlist, void *key) {
    luo_dlist_iter *iter;
    luo_dlist_node *node;

    // 获取链表迭代器,并设置从链表头开始遍历
    iter = luoDListGetIterator(dlist, LUO_DLIST_START_HEAD);

    while ((node = luoDListNext(iter)) != NULL) {
        if (dlist->match) {
            // 如果定义了匹配方法,使用匹配方法查找key
            if (dlist->match(node->value, key)) {
                // 释放迭代器并返回节点
                luoDListReleaseIterator(iter);
                return node;
            }
        } else {
            // 如果未定义匹配方法,直接根据key比较
            if (key == node->value) {
                // 释放迭代器并返回节点
                luoDListReleaseIterator(iter);
                return node;
            }
        }
    }

    // 未匹配到节点,释放迭代器并返回NULL
    luoDListReleaseIterator(iter);
    return NULL;
}

// 根据链表下标获取节点
luo_dlist_node *
luoDListIndex(luo_dlist *dlist, int index) {
    luo_dlist_node *node;

    if (index < 0) {
        // 如果下标为负数,则从后往前遍历
        index = (-index) - 1;
        node  = dlist->tail;
        while (index-- && node) {
            node = node->prev;
        }
    } else {
        // 如果下标为正数,则从前往后遍历
        node = dlist->head;
        while (index-- && node) {
            node = node->next;
        }
    }

    // 返回下标对应的节点
    return node;
}

// 重置迭代器方向,从链表头开始
void
luoDListRewind(luo_dlist *dlist) {
    // 设置迭代器下个处理节点为链表头
    dlist->iter.next      = dlist->head;
    // 设置迭代器方向从链表头开始
    dlist->iter.direction = LUO_DLIST_START_HEAD;
}

// 重置迭代器方向,从链表尾开始
void
luoDListRewindTail(luo_dlist *dlist) {
    // 设置迭代器下个处理节点为链表尾
    dlist->iter.next      = dlist->tail;
    // 设置迭代器方向从链表尾开始
    dlist->iter.direction = LUO_DLIST_START_TAIL;
}

// 根据链表定义的迭代器获取下一节点
luo_dlist_node *
luoDListYield(luo_dlist *dlist) {
    return luoDListNext(&dlist->iter);
}