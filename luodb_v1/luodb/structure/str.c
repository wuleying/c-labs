//
// Created by luoliang1 on 2017/5/4.
//

#include <luodb/structure/str.h>

luo_str_t luo_str_create(const void *init, size_t len) {
    struct luo_str *str;

    str = luo_malloc(sizeof(struct luo_str) + len + 1);

    if (str == NULL) {
        return NULL;
    }

    str->len = len;

    str->free = 0;

    if (len) {
        if (init) {
            memcpy(str->buf, init, len);
        } else {
            memset(str->buf, 0, len);
        }
    }

    str->buf[len] = '\0';

    return (char *) str->buf;
}