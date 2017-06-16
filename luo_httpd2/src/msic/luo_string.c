//
// Created by luoliang on 16/4/12.
//

#include "luo_string.h"

/**
 * 拷贝字符串
 */
char *
luo_string_dup(const char *str) {
    char   *to;
    size_t len;

    if (!str) {
        return NULL;
    }

    len = strlen(str) + 1;

    // 分配内存
    to = (char *) malloc(len);

    if (to) {
        memcpy(to, str, len);
    }

    return to;
}

/**
 * 删除字符串头尾空格
 */
unsigned
luo_string_strip(char *str) {
    char *last = NULL;
    char *dest = str;

    if (NULL == str) {
        return 0;
    }

    last = str + strlen(str);

    while (luo_isspace((int) *str) && *str) {
        str++;
    }

    while (last > str) {
        if (!luo_isspace((int) *(last - 1))) {
            break;
        }

        last--;
    }

    *last = (char) 0;

    memmove(dest, str, last - str + 1);

    return last - str;
}

/**
 * 字符串转小写
 */
const char *
luo_string_lower(const char *in, char *out, unsigned len) {
    unsigned i;

    if (NULL == in || NULL == out || 0 == len) {
        return NULL;
    }

    i = 0;

    while (in[i] != '\0' && i < len - 1) {
        out[i] = (char) tolower((int) in[i]);
        i++;
    }

    out[i] = '\0';

    return out;
}