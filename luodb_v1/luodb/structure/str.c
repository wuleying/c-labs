//
// Created by luoliang1 on 2017/5/4.
//

#include <luodb/structure/str.h>

luo_str_t luoStrCreate(const void *init, size_t len) {
    struct luo_str_s *str;

    str = luoMalloc(sizeof(struct luo_str_s) + len + 1);

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