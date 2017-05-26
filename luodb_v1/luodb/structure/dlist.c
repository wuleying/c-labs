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
#include <luodb/memory/mem.h>


luo_dlist *
luoDListCreate(void) {
    luo_dlist *dlist = luoMalloc(sizeof(luo_dlist));

    if (dlist == NULL) {
        return NULL;
    }

    dlist->head  = NULL;
    dlist->tail  = NULL;
    dlist->len   = 0;
    dlist->dup   = NULL;
    dlist->free  = NULL;
    dlist->match = NULL;

    return dlist;
}

void
luoDListRelease(luo_dlist *dlist) {
    unsigned int len;

    luo_dlist_node *current;
    luo_dlist_node *next;

    current = dlist->head;
    len     = dlist->len;

    while (len--) {
        next = current->next;
        if (dlist->free) {
            dlist->free(current->value);
        }

        luoFree(current);
        current = next;
    }

    luoFree(dlist);
}

luo_dlist *
luoDListAddNodeHead(luo_dlist *dlist, void *value) {
    luo_dlist_node *node = luoMalloc(sizeof(luo_dlist_node));

    if (node == NULL) {
        return NULL;
    }

    node->value = value;

    if (dlist->len == 0) {
        node->prev  = NULL;
        node->next  = NULL;
        dlist->head = node;
        dlist->tail = node;
    } else {
        node->prev        = NULL;
        node->next        = dlist->head;
        dlist->head->prev = node;
        dlist->head       = node;
    }

    dlist->len++;
    return dlist;
}

luo_dlist *
luoDListAddNodeTail(luo_dlist *dlist, void *value) {
    luo_dlist_node *node = luoMalloc(sizeof(luo_dlist_node));

    if (node == NULL) {
        return NULL;
    }

    node->value = value;

    if (dlist->len == 0) {
        node->prev  = NULL;
        node->next  = NULL;
        dlist->head = node;
        dlist->tail = node;
    } else {
        node->prev        = dlist->tail;
        node->next        = NULL;
        dlist->tail->next = node;
        dlist->tail       = node;
    }

    dlist->len++;
    return dlist;
}

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