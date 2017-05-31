//
// _                 _ _             _
// | |_   _  ___   __| | |__   __   _/ |
// | | | | |/ _ \ / _` | '_ \  \ \ / / |
// | | |_| | (_) | (_| | |_) |  \ V /| |
// |_|\__,_|\___/ \__,_|_.__/    \_/ |_|
//
// Created by luoliang1 on 2017/5/4.
//

#include <luodb/structure/dict.h>
#include <stdarg.h>
#include <stdio.h>
#include <luodb/memory/mem.h>
#include <assert.h>
#include <limits.h>

static void
_luoDictPanic(const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    fprintf(stderr, "\nLUO DICT LIBRARY PANIC: ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n\n");
    va_end(ap);
}

static void *
_luoDictAlloc(size_t size) {
    void *p = luoMalloc(size);

    if (p == NULL) {
        _luoDictPanic("Out of momory.");
    }

    return p;
}

static void
_luoDictFree(void *ptr) {
    luoFree(ptr);
}

static void
_luoDictReset(luo_dict *dict) {
    dict->table     = NULL;
    dict->size      = 0;
    dict->size_mask = 0;
    dict->used      = 0;
}

static int
_luoDictInit(luo_dict *dict, luo_dict_type *type, void *priv_data_ptr) {
    _luoDictReset(dict);
    dict->type      = type;
    dict->priv_data = priv_data_ptr;

    return LUO_DICT_OK;
}

static unsigned long
_luoDictNextPower(unsigned long size) {
    unsigned long i = LUO_DICT_INITIAL_SIZE;

    if (size >= LONG_MAX) {
        return LONG_MAX;
    }

    while (1) {
        if (i >= size) {
            return i;
        }
        i *= 2;
    }
}

static int
_luoDictExpandIfNeeded(luo_dict *dict) {
    if (dict->size == 0) {
        return luoDictExpand(dict, LUO_DICT_INITIAL_SIZE);
    }

    if (dict->used == dict->size) {
        return luoDictExpand(dict, dict->size * 2);
    }

    return LUO_DICT_OK;
}

static int
_luoDictKeyIndex(luo_dict *dict, const void *key) {
    unsigned int   h;
    luo_dict_entry *dict_entry;

    if (_luoDictExpandIfNeeded(dict) == LUO_DICT_ERR) {
        return -1;
    }

    h = (unsigned int) (LUO_DICT_HASH_KEY(dict, key) & dict->size_mask);

    dict_entry = dict->table[h];

    while (dict_entry) {
        if (LUO_DICT_COMPARE_HASH_KEYS(dict, key, dict_entry->key)) {
            return -1;
        }

        dict_entry = dict_entry->next;
    }

    return h;
}

static int
_luoDictGenericDelete(luo_dict *dict, const void *key, int nofree) {
    unsigned int   h;
    luo_dict_entry *dict_entry;
    luo_dict_entry *dict_entry_prev = NULL;

    if (dict->size == 0) {
        return LUO_DICT_ERR;
    }

    h = (unsigned int) (LUO_DICT_HASH_KEY(dict, key) & dict->size_mask);

    dict_entry = dict->table[h];

    while (dict_entry) {
        if (LUO_DICT_COMPARE_HASH_KEYS(dict, key, dict_entry->key)) {
            if (dict_entry_prev) {
                dict_entry_prev->next = dict_entry->next;
            } else {
                dict->table[h] = dict_entry->next;
            }

            if (!nofree) {
                LUO_DICT_FREE_ENTRY_KEY(dict, dict_entry);
                LUO_DICT_FREE_ENTRY_VAL(dict, dict_entry);
            }

            _luoDictFree(dict_entry);
            dict->used--;

            return LUO_DICT_OK;
        }

        dict_entry_prev = dict_entry;
        dict_entry      = dict_entry->next;
    }

    return LUO_DICT_ERR;
}

int
_luoDictClear(luo_dict *dict) {
    unsigned long i;

    for (i = 0; i < dict->size && dict->used > 0; i++) {
        luo_dict_entry *dict_entry;
        luo_dict_entry *dict_entry_next;

        dict_entry = dict->table[i];

        if (dict_entry == NULL) {
            continue;
        }

        while (dict_entry) {
            dict_entry_next = dict_entry->next;

            LUO_DICT_FREE_ENTRY_KEY(dict, dict_entry);
            LUO_DICT_FREE_ENTRY_VAL(dict, dict_entry);

            _luoDictFree(dict_entry);
            dict->used--;
            dict_entry = dict_entry_next;
        }
    }

    _luoDictFree(dict->table);
    _luoDictReset(dict);

    return LUO_DICT_OK;
}

unsigned int
luoDictIntHashFunction(unsigned int key) {
    key += ~(key << 15);
    key ^= (key >> 10);
    key += (key << 3);
    key ^= (key >> 6);
    key += ~(key << 11);
    key ^= (key >> 16);
    return key;
}

unsigned int
luoDictIdentityHashFunction(unsigned int key) {
    return key;
}

unsigned int
luoDictGenHashFunction(const unsigned char *buf, int len) {
    unsigned int hash = 5381;

    while (len--) {
        // hash * 33 + c
        hash = ((hash << 5) + hash) + (*buf++);
    }

    return hash;
}

/* 接口实现 */
luo_dict *
luoDictCreate(luo_dict_type *type, void *priv_data_ptr) {
    luo_dict *dict = _luoDictAlloc(sizeof(luo_dict));

    _luoDictInit(dict, type, priv_data_ptr);

    return dict;
}

int
luoDictExpand(luo_dict *dict, unsigned long size) {
    luo_dict      new_dict;
    unsigned long real_size = _luoDictNextPower(size);
    unsigned long i;

    if (dict->used > size) {
        return LUO_DICT_ERR;
    }

    _luoDictInit(&new_dict, dict->type, dict->priv_data);
    new_dict.size      = real_size;
    new_dict.size_mask = real_size - 1;
    new_dict.table     = _luoDictAlloc(real_size * sizeof(luo_dict_entry *));

    memset(new_dict.table, 0, real_size * sizeof(luo_dict_entry *));

    new_dict.used = dict->used;

    for (i = 0; i < dict->size && dict->used > 0; i++) {
        luo_dict_entry *dict_entry;
        luo_dict_entry *dict_entry_next;

        if (dict->table[i] == NULL) {
            continue;
        }

        dict_entry = dict->table[i];

        while (dict_entry) {
            unsigned int h;

            dict_entry_next = dict_entry->next;

            h = (unsigned int) (LUO_DICT_HASH_KEY(dict, dict_entry->key) & new_dict.size_mask);

            dict_entry->next = new_dict.table[h];

            dict->used--;

            dict_entry = dict_entry_next;
        }
    }

    assert(dict->used == 0);
    _luoDictFree(dict->table);
    *dict = new_dict;

    return LUO_DICT_OK;
}

int
luoDictAdd(luo_dict *dict, void *key, void *val) {
    int index;

    luo_dict_entry *entry;

    if ((index = _luoDictKeyIndex(dict, key)) == -1) {
        return LUO_DICT_ERR;
    }

    entry = _luoDictAlloc(sizeof(luo_dict_entry));

    entry->next = dict->table[index];

    dict->table[index] = entry;

    LUO_DICT_SET_HASH_KEY(dict, entry, key);
    LUO_DICT_SET_HASH_VAL(dict, entry, val);

    dict->used++;

    return LUO_DICT_OK;
}

int
luoDictReplace(luo_dict *dict, void *key, void *val) {
    luo_dict_entry *entry;

    if (luoDictAdd(dict, key, val) == LUO_DICT_OK) {
        return LUO_DICT_OK;
    }

    entry = luoDictFind(dict, key);

    LUO_DICT_FREE_ENTRY_VAL(dict, entry);
    LUO_DICT_SET_HASH_VAL(dict, entry, val);

    return LUO_DICT_OK;
}

int
luoDictDelete(luo_dict *dict, const void *key) {
    return _luoDictGenericDelete(dict, key, 0);
}

int
luoDictDeleteNoFree(luo_dict *dict, const void *key) {
    return _luoDictGenericDelete(dict, key, 1);
}

void
luoDictRelease(luo_dict *dict) {
    _luoDictClear(dict);
    _luoDictFree(dict);
}

luo_dict_entry *
luoDictFind(luo_dict *dict, const void *key) {
    luo_dict_entry *dict_entry;
    unsigned int   h;

    if (dict->size == 0) {
        return NULL;
    }

    h = (unsigned int) (LUO_DICT_HASH_KEY(dict, key) && dict->size_mask);

    dict_entry = dict->table[h];

    while (dict_entry) {
        if (LUO_DICT_COMPARE_HASH_KEYS(dict, key, dict_entry->key)) {
            return dict_entry;
        }

        dict_entry = dict_entry->next;
    }

    return NULL;
}