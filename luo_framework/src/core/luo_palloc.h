//
// Created by luoliang on 16/6/13.
//

#ifndef LUO_FRAMEWORK_LUO_PALLOC_H
#define LUO_FRAMEWORK_LUO_PALLOC_H

#include "luo_config.h"
#include "luo_core.h"

// 内存池最大分配空间
#define LUO_MAX_ALLOC_FROM_POOL 4095
// 内存池默认大小
#define LUO_DEFAULT_POOL_SIZE   (16 * 1024)
// 内存池对齐
#define LUO_POOL_ALIGNMENT      16
// 内存池最小大小
#define LUO_MIN_POOL_SIZE       luo_align(sizeof(luo_pool_t) + 2 * sizeof(luo_pool_large_t), LUO_POOL_ALIGNMENT);

#define lou_memalign(alignment, size)   luo_alloc(size)
#define luo_free                        free

typedef void (*luo_pool_cleanup_pt)(void *data);

// 释放内存池结构体
typedef struct luo_pool_cleanup_s luo_pool_cleanup_t;
struct luo_pool_cleanup_s {
    luo_pool_cleanup_pt handler;
    void                *data;
    luo_pool_cleanup_t  *next;
};

// todo
typedef struct luo_pool_large_s luo_pool_large_t;
struct luo_pool_large_s {
    luo_pool_large_t *next;
    void             *alloc;
};

// 内存池数据结构体
typedef struct luo_pool_data_s luo_pool_data_t;
struct luo_pool_data_s {
    luo_uchar_t *last;
    luo_uchar_t *end;
    luo_pool_t  *next;
    luo_uint_t  failed;
};

// 内存池结构体
struct luo_pool_s {
    luo_pool_data_t    data;
    size_t             max;
    luo_pool_t         *current;
    luo_chain_t        *chain;
    luo_pool_large_t   *large;
    luo_pool_cleanup_t *cleanup;
};

// 分配内存
void *luo_alloc(size_t size);

// 分配内存并初始化为0
void *luo_calloc(size_t size);

// 创建内存池
luo_pool_t *luo_pool_create(size_t size);

// 释放内存池
void luo_destroy_pool(luo_pool_t *pool);

// 重置内存池
void luo_reset_pool(luo_pool_t *pool);

// 通过内存池分配内存
void *luo_palloc(luo_pool_t *pool, size_t size);

// 通过内存池分配内存
void *luo_pnalloc(luo_pool_t *pool, size_t size);

// 通过内存池分配内存
void *luo_pcalloc(luo_pool_t *pool, size_t size);

#endif //LUO_FRAMEWORK_LUO_PALLOC_H
