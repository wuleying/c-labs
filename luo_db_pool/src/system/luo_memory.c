//
// Created by luoliang on 16/7/8.
//

#include "../core/luo_core.h"

// 分配内存
void *
luo_memory_alloc(size_t size) {
    void *p = malloc(size);

    if (!p) {
        // todo error
    }

    return p;
}

// 分配内存
void *
luo_memory_calloc(size_t count, size_t size) {
    void *p = calloc(count, size);

    if (!p) {
        // todo error
    }

    return p;
}

// 释放内存
void
luo_memory_free(void *p) {
    if (p) {
        free(p);
    }
}

// 重置内存
void *
luo_memory_reset(void *p, size_t size) {
    p = realloc(p, size);

    if (!p) {
        // todo error
    }

    return p;
}