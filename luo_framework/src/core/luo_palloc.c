//
// Created by luoliang on 16/6/13.
//

#include "luo_config.h"
#include "luo_core.h"

// 分配内存
void *
luo_alloc(size_t size) {
    void *p;
    p = malloc(size);

    return p;
}

// 分配内存并初始化为0
void *
luo_calloc(size_t size) {
    void *p;

    p = luo_alloc(size);

    if (p) {
        luo_memzero(p, size);
    }

    return p;
}

// 创建内存池
luo_pool_t *
luo_pool_create(size_t size) {
    luo_pool_t *p;

    p = lou_memalign(LUO_POOL_ALIGNMENT, size);

    if (NULL == p) {
        return NULL;
    }

    p->data.last   = (luo_uchar_t *) p + sizeof(luo_pool_t);
    p->data.end    = (luo_uchar_t *) p + size;
    p->data.next   = NULL;
    p->data.failed = 0;

    size = size - sizeof(luo_pool_t);

    p->max = (size < LUO_MAX_ALLOC_FROM_POOL) ? size : LUO_MAX_ALLOC_FROM_POOL;

    p->current = p;
    p->chain   = NULL;
    p->large   = NULL;
    p->cleanup = NULL;

    return p;
}

// 释放内存池
void
luo_destroy_pool(luo_pool_t *pool) {
    luo_pool_t         *p, *n;
    luo_pool_large_t   *l;
    luo_pool_cleanup_t *c;

    for (c = pool->cleanup; c; c = c->next) {
        if (c->handler) {
            c->handler(c->data);
        }
    }

    for (l = pool->large; l; l = l->next) {
        if (l->alloc) {
            luo_free(l->alloc);
        }
    }

    for (p = pool, n = pool->data.next; ; p = n, n = n->data.next) {
        luo_free(p);

        if (NULL == n) {
            break;
        }
    }
}

// 重置内存池
void
luo_reset_pool(luo_pool_t *pool) {
    luo_pool_t       *p;
    luo_pool_large_t *l;

    for (l = pool->large; l; l = l->next) {
        if (l->alloc) {
            luo_free(l->alloc);
        }
    }

    for (p = pool; p; p = pool->data.next) {
        p->data.last   = (luo_uchar_t *) p + sizeof(luo_pool_t);
        p->data.failed = 0;
    }

    pool->current = pool;
    pool->chain   = NULL;
    pool->large   = NULL;
}

// 通过内存池分配内存
void *
luo_palloc(luo_pool_t *pool, size_t size) {

}

// 通过内存池分配内存
void *
luo_pnalloc(luo_pool_t *pool, size_t size) {

}

// 通过内存池分配内存
void *
luo_pcalloc(luo_pool_t *pool, size_t size) {

}