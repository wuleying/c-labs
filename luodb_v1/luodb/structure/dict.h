//
// _                 _ _             _
// | |_   _  ___   __| | |__   __   _/ |
// | | | | |/ _ \ / _` | '_ \  \ \ / / |
// | | |_| | (_) | (_| | |_) |  \ V /| |
// |_|\__,_|\___/ \__,_|_.__/    \_/ |_|
//
// Created by luoliang1 on 2017/5/4.
//

#ifndef LUODB_DICT_H
#define LUODB_DICT_H

#include <stdarg.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>

#include <luodb/common/constant.h>
#include <luodb/common/macro.h>
#include <luodb/memory/mem.h>

/* 结构体 */
// 字典元素,保存k-v值
typedef struct luo_dict_entry_s {
    // 键指针
    void                    *key;
    // 值指针
    void                    *val;
    // 后继字典元素指针
    struct luo_dict_entry_s *next;
} luo_dict_entry;

// 字典类型
typedef struct luo_dict_type_s {
    // 哈希计算方法,返回无符号整型
    unsigned int (*hashFunction)(const void *key);

    // 复制键函数指针
    void *(*key_dup)(void *priv_data, const void *key);

    // 复制值函数指针
    void *(*val_dup)(void *priv_data, const void *object);

    // 键比较函数指针
    int          (*key_compare)(void *priv_data, const void *key1, const void *key2);

    // 键的析构函数指针
    void         (*key_destructor)(void *priv_data, void *key);

    // 值的析构函数指针
    void         (*val_destructor)(void *priv_data, void *object);
} luo_dict_type;

// 字典哈希表
typedef struct luo_dict_s {
    // 字典数组 bucket
    luo_dict_entry **table;
    // 字典类型
    luo_dict_type  *type;
    // bucket的层数
    unsigned long  size;
    unsigned long  size_mask;
    // 字典已使用数量
    unsigned long  used;
    // 私有数据指针
    void           *priv_data;
} luo_dict;

// 字典迭代器
typedef struct luo_dict_iterator_s {
    // 当前字典
    luo_dict       *dict;
    // 下标
    int            index;
    // 当前字典数组
    luo_dict_entry *entry;
    // 当前字典数组后继
    luo_dict_entry *entry_next;
} luo_dict_iterator;

/* 常量 */
// 字典初始化时bucket的数量
#define LUO_DICT_INITIAL_SIZE   16
// 调试用 clvector 长度
#define LUO_DICT_STATS_VECTLEN  50

// 调用字典定义的val释放函数
#define LUO_DICT_FREE_ENTRY_VAL(dict, entry)                            \
    if((dict)->type->val_destructor)                                    \
        (dict)->type->val_destructor((dict)->priv_data, (entry)->val)

// 调用字典定义的val复制函数,未定义直接复制val
#define LUO_DICT_SET_HASH_VAL(dict, entry, _val_) do {                  \
    if((dict)->type->val_dup)                                           \
        entry->val = (dict)->type->val_dup((dict)->priv_data, _val_);   \
    else                                                                \
        entry->val = (_val_);                                           \
} while(0)

// 调用字典定义的key释放函数
#define LUO_DICT_FREE_ENTRY_KEY(dict, entry)                            \
    if((dict)->type->key_destructor)                                    \
        (dict)->type->key_destructor((dict)->priv_data, (entry)->key)

// 调用字典定义的key复制函数,未定义直接复制key
#define LUO_DICT_SET_HASH_KEY(dict, entry, _key_) do {                  \
    if((dict)->type->key_dup)                                           \
        entry->key = (dict)->type->key_dup((dict)->priv_data, _key_);   \
    else                                                                \
        entry->key = (_key_);                                           \
} while(0)

// 调用字典定义的key比较函数,未定义直接比较key
#define LUO_DICT_COMPARE_HASH_KEYS(dict, key1, key2)                    \
    (((dict)->type->key_compare) ?                                      \
        (dict)->type->key_compare((dict)->priv_data, key1, key2) :      \
        (key1) == (key2))

// 哈希定位函数
#define LUO_DICT_HASH_KEY(dict, key) (dict)->type->hashFunction(key)

// 获取字典元素的key
#define LUO_DICT_GET_ENTRY_KEY(dict_entry) ((dict_entry)->key)

// 获取字典元素的val
#define LUO_DICT_GET_ENTRY_VAL(dict_entry) ((dict_entry)->val)

// 获取字典bucket的层数
#define LUO_DICT_GET_SIZE(dict) ((dict)->size)

// 获取字典已使用数量
#define LUO_DICT_GET_USED(dict) ((dict)->used)

/* 接口 */
luo_dict *luoDictCreate(luo_dict_type *type, void *priv_data_ptr);

int luoDictExpand(luo_dict *dict, unsigned long size);

int luoDictAdd(luo_dict *dict, void *key, void *val);

int luoDictReplace(luo_dict *dict, void *key, void *val);

int luoDictDelete(luo_dict *dict, const void *key);

int luoDictDeleteNoFree(luo_dict *dict, const void *key);

void luoDictRelease(luo_dict *dict);

luo_dict_entry *luoDictFind(luo_dict *dict, const void *key);

int luoDictResize(luo_dict *dict);

luo_dict_iterator *luoDictGetInterator(luo_dict *dict);

luo_dict_entry *luoDictNext(luo_dict_iterator *iter);

void luoDictReleaseIterator(luo_dict_iterator *iter);

luo_dict_entry *luoDictGetRandomKey(luo_dict *dict);

void luoDictPrintStats(luo_dict *dict);

unsigned int luoDictGenHashFunction(const unsigned char *buf, int len);

void luoDictEmpty(luo_dict *dict);

/* 哈希表类型 */
extern luo_dict_type luoDictTypeHeapStringCopyKey;
extern luo_dict_type luoDictTypeHeapStrings;
extern luo_dict_type luoDictTypeHeapStringCopyKeyValue;

#endif //LUODB_DICT_H
