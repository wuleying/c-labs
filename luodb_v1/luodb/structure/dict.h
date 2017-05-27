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

/* 结构体 */
typedef struct luo_dict_entry_s {
    void                    *key;
    void                    *val;
    struct luo_dict_entry_s *next;
} luo_dict_entry;

typedef struct luo_dict_type_s {
    unsigned int (*hashFunction)(const void *key);

    void *(*key_dup)(void *priv_data, const void *key);

    void *(*val_dup)(void *priv_data, const void *object);

    int          (*val_compare)(void *priv_data, const void *key1, const void *key2);

    void         (*key_destructor)(void *priv_data, void *key);

    void         (*val_destructor)(void *priv_data, void *object);
} luo_dict_type;

typedef struct luo_dict_s {
    luo_dict_entry **table;
    luo_dict_type  *type;
    unsigned long  size;
    unsigned long  size_mask;
    unsigned long  used;
    void           *priv_data;
} luo_dict;

typedef struct luo_dict_iterator_s {
    luo_dict       *dict;
    int            index;
    luo_dict_entry *entry;
    luo_dict_entry *next_entry;
} luo_dict_iterator;

/* 常量 */
#define LUO_DICT_OK             0
#define LUO_DICT_ERR            1

#define LUO_DICT_INITIAL_SIZE   16

/* 宏定义 */
#define LUO_DICT_NOT_USED(V) ((void) V)

#define LUO_DICT_FREE_ENTRY_VAL(dict, entry)                            \
    if((dict)->type->val_destructor)                                    \
        (dict)->type->val_destructor((dict)->priv_data, (entry)->val)

#define LUO_DICT_SET_HASH_VAL(dict_entry, _val_) do {                   \
    if((dict)->type->val_dup)                                           \
        entry->val = (dict)->type=>val_dup((dict)->priv_data, _val_);   \
    else                                                                \
        entry->val = (_val_);                                           \
} while(0)

#define LUO_DICT_FREE_ENTRY_KEY(dict, entry)                            \
    if((dict)->type->key_destructor)                                    \
        (dict)->type->key_destructor((dict)->priv_data, (entry)->key)

#define LUO_DICT_SET_HASH_KEY(dict, entry, _key_) do {                  \
    if((dict)->type->key_dup)                                           \
        entry->key = (dict)->type->key_dup((dict)->priv_data, _key_);   \
    else                                                                \
        entry->key = (_key_);                                           \
} while(0)

#define dict_compare_hash_keys(dict, key1, key2)                        \
    (((dict)->type->key_compare) ?                                      \
        (dict)->type->key_compare((dict)->priv_data, key1, key2) :      \
        (key1) == (key2))

#endif //LUODB_DICT_H
