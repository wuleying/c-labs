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

// 已使用内存
static size_t _used_memory = 0;

// 分配一块指定大小的内存区域
void *
luoMalloc(size_t size) {
    // 申请内存
    void *ptr = malloc(size + sizeof(size_t));

    // 申请失败直接返回NULL
    if (!ptr) {
        return NULL;
    }

    *((size_t *) ptr) = size;

    // 更新已使用内存
    _used_memory += size + sizeof(size_t);

    // 返回指向该区域头部指针
    return (char *) ptr + sizeof(size_t);
}

// 对luoMalloc申请的内存进行动态大小调整
void *
luoRealloc(void *ptr, size_t size) {
    // 原始内存头部指针
    void   *real_ptr;
    // 重新分配内存头部指针
    void   *new_ptr;
    // 原始内存空间大小
    size_t old_size;

    // 原始内存头部指针为空,直接分配指定大小内存
    if (ptr == NULL) {
        return luoMalloc(size);
    }

    // 获取原始内存头部指针
    real_ptr = (char *) ptr - sizeof(size_t);
    // 获取原始内存空间大小
    old_size = *((size_t *) real_ptr);
    // 重新分配内存
    new_ptr  = realloc(real_ptr, size + sizeof(size_t));

    // 分配内存失败,直接返回空
    if (!new_ptr) {
        return NULL;
    }

    *((size_t *) new_ptr) = size;

    // 更新内存使用量
    _used_memory -= old_size;
    _used_memory += size;

    // 返回指向重分配内存区域头部指针
    return (char *) new_ptr + sizeof(size_t);
}

// 释放ptr指向的存储空间
void
luoFree(void *ptr) {
    // 内存区域头部指针
    void   *real_ptr;
    // 内存空间大小
    size_t old_size;

    // 内存区域头部指针为空,直接返回
    if (ptr == NULL) {
        return;
    }

    real_ptr = (char *) ptr - sizeof(size_t);
    old_size = *((size_t *) real_ptr);

    // 更新内存使用量
    _used_memory -= old_size + sizeof(size_t);

    // 释放内存
    free(real_ptr);
}

// 拷贝字符串内存
char *
luoStrdup(const char *str) {
    // 字符串长度
    size_t len  = strlen(str) + 1;
    // 申请内存
    char   *ptr = luoMalloc(len);

    // 拷贝字符串
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