//
// Created by luoliang on 16/6/13.
//

#ifndef LUO_FRAMEWORK_LUO_HASHTABLE_H
#define LUO_FRAMEWORK_LUO_HASHTABLE_H

#include "luo_config.h"
#include "luo_core.h"

// 哈希表节点结构体
typedef struct luo_hashtable_node_s luo_hashtable_node_t;
struct luo_hashtable_node_s {
    struct luo_hashtable_node_t *next;
    luo_char_t                  *key;
    void                        *val;
};

// 哈希表结构体
typedef struct luo_hashtable_s luo_hashtable_t;
struct luo_hashtable_s {
    luo_uint_t                  size;
    luo_uint_t                  count;
    struct luo_hashtable_node_t *node;
};

#endif //LUO_FRAMEWORK_LUO_HASHTABLE_H
