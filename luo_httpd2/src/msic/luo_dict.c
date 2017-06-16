//
// Created by luoliang on 16/4/14.
//

#include "luo_dict.h"

/**
 * 创建一个字典
 */
luo_dict_t *
luo_dict_create(size_t size) {
    luo_dict_t *dict;

    if (size < DICT_MIN_SIZE) {
        size = DICT_MIN_SIZE;
    }

    dict = (luo_dict_t *) calloc(1, sizeof *dict);

    if (dict) {
        dict->size = size;
        dict->val  = (char **) calloc(size, sizeof *dict->val);
        dict->key  = (char **) calloc(size, sizeof *dict->key);
        dict->hash = (unsigned *) calloc(size, sizeof *dict->hash);
    }

    return dict;
}

/**
 * 删除字典
 */
void
luo_dict_delete(luo_dict_t *dict) {
    ssize_t i;

    if (NULL == dict) {
        return;
    }

    for (i = 0; i < dict->size; i++) {
        if (NULL != dict->key[i]) {
            free(dict->key[i]);
        }

        if (NULL != dict->val[i]) {
            free(dict->val[i]);
        }
    }

    free(dict->key);
    free(dict->val);
    free(dict->hash);
    free(dict);

    return;
}

/**
 * 计算关键词的哈希值
 */
unsigned
luo_dict_hash(const char *key) {
    size_t   len;
    size_t   i;
    unsigned hash;

    if (!key) {
        return 0;
    }

    len = strlen(key);

    for (hash = 0, i = 0; i < len; i++) {
        hash += (unsigned) key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}

/**
 * 设置值
 */
int
luo_dict_set(luo_dict_t *dict, const char *key, const char *val) {
    ssize_t  i;
    unsigned hash;

    if (NULL == dict || NULL == key) {
        return -1;
    }

    hash = luo_dict_hash(key);

    if (dict->n > 0) {
        for (i = 0; i < dict->size; i++) {
            if (NULL == dict->key[i]) {
                continue;
            }

            if (hash == dict->hash[i]) {
                if (!strcmp(key, dict->key[i])) {
                    if (NULL != dict->val[i]) {
                        free(dict->val[i]);
                    }

                    dict->val[i] = (val ? luo_string_dup(val) : NULL);

                    return 0;
                }
            }
        }
    }

    // 字典扩容
    if (dict->n == dict->size) {
        if (0 != luo_dict_grow(dict)) {
            return -1;
        }
    }

    for (i = dict->n; dict->key[i];) {
        if (++i == dict->size) {
            i = 0;
        }
    }

    dict->key[i]  = luo_string_dup(key);
    dict->val[i]  = (val ? luo_string_dup(val) : NULL);
    dict->hash[i] = hash;
    dict->n++;

    return 0;
}

/**
 * 获取值
 */
const char *
luo_dict_get(luo_dict_t *dict, const char *key) {
    unsigned hash;
    ssize_t  i;

    hash = luo_dict_hash(key);

    for (i = 0; i < dict->size; i++) {
        if (NULL == dict->key[i]) {
            continue;
        }

        if (hash == dict->hash[i]) {
            if (!strcmp(key, dict->key[i])) {
                return dict->val[i];
            }
        }
    }

    return NULL;
}

/**
 * 字典扩容(原大小的2倍)
 */
static int
luo_dict_grow(luo_dict_t *dict) {
    char     **new_key;
    char     **new_val;
    unsigned *new_hash;

    // 分配新的内存空间
    new_key    = (char **) calloc(dict->size * 2, sizeof *dict->key);
    new_val    = (char **) calloc(dict->size * 2, sizeof *dict->val);
    new_hash   = (unsigned *) calloc(dict->size * 2, sizeof *dict->hash);

    // 分配内存失败
    if (!new_key || !new_val || !new_hash) {
        if (new_key) {
            free(new_key);
        }

        if (new_val) {
            free(new_val);
        }

        if (new_hash) {
            free(new_hash);
        }

        return -1;
    }

    // 拷贝字典内容
    memcpy(new_key, dict->key, dict->size * sizeof(char *));
    memcpy(new_val, dict->val, dict->size * sizeof(char *));
    memcpy(new_hash, dict->hash, dict->size * sizeof(unsigned));

    // 释放内存
    free(dict->key);
    free(dict->val);
    free(dict->hash);

    // 扩展字典 大小为原字典的两倍
    dict->size *= 2;
    dict->key  = new_key;
    dict->val  = new_val;
    dict->hash = new_hash;

    return 0;
}