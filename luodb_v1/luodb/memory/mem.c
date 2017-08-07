//
// _                 _ _             _
// | |_   _  ___   __| | |__   __   _/ |
// | | | | |/ _ \ / _` | '_ \  \ \ / / |
// | | |_| | (_) | (_| | |_) |  \ V /| |
// |_|\__,_|\___/ \__,_|_.__/    \_/ |_|
//
// Created by luoliang1 on 2017/5/4.
//

#include <luodb/memory/mem.h>

// 已分配内存
static size_t _used_memory = 0;

// 分配一块指定大小的内存区域，并返回指向该区域头部的指针，分配失败则返回NULL
void *
luoMalloc(size_t size) {
    void *ptr = malloc(size + sizeof(size_t));

    if (!ptr) {
        return NULL;
    }

    *((size_t *) ptr) = size;
    _used_memory += size + sizeof(size_t);

    return (char *) ptr + sizeof(size_t);
}

// 对luoMalloc申请的内存进行动态大小调整
void *
luoRealloc(void *ptr, size_t size) {
    void   *real_ptr;
    void   *new_ptr;
    size_t old_size;

    if (ptr == NULL) {
        return luoMalloc(size);
    }

    real_ptr = (char *) ptr - sizeof(size_t);
    old_size = *((size_t *) real_ptr);
    new_ptr  = realloc(real_ptr, size + sizeof(size_t));

    if (!new_ptr) {
        return NULL;
    }

    *((size_t *) new_ptr) = size;
    _used_memory -= old_size;
    _used_memory += size;

    return (char *) new_ptr + sizeof(size_t);
}

// 释放ptr指向的存储空间
void
luoFree(void *ptr) {
    void   *real_ptr;
    size_t old_size;

    if (ptr == NULL) {
        return;
    }

    real_ptr = (char *) ptr - sizeof(size_t);
    old_size = *((size_t *) real_ptr);
    _used_memory -= old_size + sizeof(size_t);

    free(real_ptr);
}

// 拷贝字符串内存
char *
luoStrdup(const char *str) {
    size_t len  = strlen(str) + 1;
    char   *ptr = luoMalloc(len);

    memcpy(ptr, str, len);

    return ptr;
}

// 内存溢出
void
luoOom(const char *msg) {
    fprintf(stderr, "%s: Out of memory\n", msg);
    fflush(stderr);
    sleep(1);
    abort();
}

// 已使用内存大小
size_t
luoMallocUsedMemory(void) {
    return _used_memory;
}