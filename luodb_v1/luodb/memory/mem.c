//
// Created by luoliang1 on 2017/5/4.
//

#include "mem.h"

static size_t used_memory = 0;

void *luo_malloc(size_t size) {
    void *ptr = malloc(size + sizeof(size_t));

    if (!ptr) {
        return NULL;
    }

    *((size_t *) ptr) = size;
    used_memory += size + sizeof(size_t);

    return (char *) ptr + sizeof(size_t);
}
