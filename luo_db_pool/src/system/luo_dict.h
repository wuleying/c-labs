//
// Created by luoliang on 16/7/12.
//

#ifndef LUO_DB_POOL_LUO_DICT_H
#define LUO_DB_POOL_LUO_DICT_H

#include "../core/luo_core.h"

#define DICT_MIN_SIZE       (128)

/**
 * 字典结构体
 */
typedef struct luo_dict_s {
    int      n;
    ssize_t  size;
    char     **key;
    char     **val;
    unsigned *hash;
} luo_dict_t;

/**
 * 创建字典
 */
luo_dict_t *luo_dict_create(size_t size);

/**
 * 删除字典
 */
void luo_dict_delete(luo_dict_t *dict);

/**
 * 计算关键词的哈希值
 */
unsigned luo_dict_hash(const char *key);

/**
 * 设置值
 */
int luo_dict_set(luo_dict_t *dict, const char *key, const char *val);

/**
 * 获取值
 */
const char *luo_dict_get(luo_dict_t *dict, const char *key);

/**
 * 字典扩容(原大小的2倍)
 */
static int luo_dict_grow(luo_dict_t *dict);

#endif //LUO_DB_POOL_LUO_DICT_H
