//
// Created by luoliang1 on 2017/5/4.
//

#include <luodb/memory/mem.h>

static size_t used_memory = 0;

// 分配一块指定大小的内存区域，并返回指向该区域头部的指针，分配失败则返回NULL
void *
luo_malloc(size_t size) {
    void *ptr = malloc(size + sizeof(size_t));

    if (!ptr) {
        return NULL;
    }

    *((size_t *) ptr) = size;
    used_memory += size + sizeof(size_t);

    return (char *) ptr + sizeof(size_t);
}

// 对luo_malloc申请的内存进行动态大小调整
void *
luo_realloc(void *ptr, size_t size) {
    void   *real_ptr;
    void   *new_ptr;
    size_t old_size;

    if (ptr == NULL) {
        return luo_malloc(size);
    }

    real_ptr = (char *) ptr - sizeof(size_t);
    old_size = *((size_t *) real_ptr);
    new_ptr  = realloc(real_ptr, size + sizeof(size_t));

    if (!new_ptr) {
        return NULL;
    }

    *((size_t *) new_ptr) = size;
    used_memory -= old_size;
    used_memory += size;

    return (char *) new_ptr + sizeof(size_t);
}

// 释放ptr指向的存储空间
void
luo_free(void *ptr) {
    void   *real_ptr;
    size_t old_size;

    if (ptr == NULL) {
        return;
    }

    real_ptr = (char *) ptr - sizeof(size_t);
    old_size = *((size_t *) real_ptr);
    used_memory -= old_size + sizeof(size_t);

    free(real_ptr);
}

// 拷贝字符串内存
char *
luo_strdup(const char *str) {
    size_t len  = strlen(str) + 1;
    char   *ptr = luo_malloc(len);

    memcpy(ptr, str, len);

    return ptr;
}

// 返回已使用内存大小
size_t
luo_malloc_used_memory(void) {
    return used_memory;
}