//
// Created by luoliang on 16/7/8.
//

#ifndef PROJECT_LUO_MEMORY_H
#define PROJECT_LUO_MEMORY_H

// 分配内存
#define luo_alloc(size) luo_memory_alloc(size)
// 分配内存
#define luo_calloc(count, size) luo_memory_calloc(count, size)
// 分配内存
#define luo_new(p) (p = LUO_CALLOC(1, sizeof *(p)))
// 释放内存
#define luo_free(p) ((void)luo_memory_free(p))
// 重置内存
#define luo_reset(p, size) (p = luo_memory_reset(p, size))

// 分配内存
void *luo_memory_alloc(size_t size);

// 分配内存
void *luo_memory_calloc(size_t count, size_t size);

// 释放内存
void luo_memory_free(void *p);

// 重置内存
void *luo_memory_reset(void *p, size_t size);

#endif //PROJECT_LUO_MEMORY_H