//
//  _                 _ _             _
// | |_   _  ___   __| | |__   __   _/ |
// | | | | |/ _ \ / _` | '_ \  \ \ / / |
// | | |_| | (_) | (_| | |_) |  \ V /| |
// |_|\__,_|\___/ \__,_|_.__/    \_/ |_|
//
// Created by luoliang1 on 2017/5/4.
//

#include <luodb/structure/str.h>

// 生成一个指定值与长度的luo_str
luo_str
luoStrCreate(const void *init, long len) {
    // 字符串
    struct luo_str_s *str;

    // 分配内存空间
    str = luoMalloc(sizeof(struct luo_str_s) + len + 1);

    // 分配内存失败,返回NULL
    if (str == NULL) {
        return NULL;
    }

    // 设置字符串长度
    str->len = len;

    // 设置字符串可用空间
    str->free = 0;

    if (len) {
        if (init) {
            // 将init内容拷贝到字符串buffer中
            memcpy(str->buf, init, len);
        } else {
            // 将buffer的前len个字节指定为0
            memset(str->buf, 0, len);
        }
    }

    // buffer加上字符串结束符\0
    str->buf[len] = '\0';

    // 返回buffer字符串指针
    return (char *) str->buf;
}

// 生成指定值的luo_str,无需指定长度
luo_str
luoStrNew(const char *init) {
    // 获取字符串长度
    size_t len = (init == NULL) ? 0 : strlen(init);
    // 返回字符串
    return luoStrCreate(init, len);
}

// 生成一个长度为0的空字符串
luo_str
luoStrEmpty() {
    return luoStrCreate("", 0);
}

// 返回一字luo_str字符串的长度
int
luoStrLen(const luo_str ls) {
    struct luo_str_s *lss = (void *) (ls - LUO_STR_LEN);
    return (int) lss->len;
}

luo_str
luoStrDup(const luo_str ls) {
    return luoStrCreate(ls, luoStrLen(ls));
}

void
luoStrFree(luo_str ls) {
    if (ls == NULL) {
        return;
    }

    luoFree(ls - sizeof(struct luo_str_s));
}

long
luoStrAvail(luo_str ls) {
    struct luo_str_s *lss = (void *) (ls - LUO_STR_LEN);

    return lss->free;
}

static luo_str
luoStrMakeRoomFor(luo_str ls, long add_len) {
    struct luo_str_s *lss, *new_lss;

    long free = luoStrAvail(ls);
    long len, new_len;

    if (free >= add_len) {
        return ls;
    }

    len     = luoStrLen(ls);
    lss     = (void *) (ls - LUO_STR_LEN);
    new_len = (len + add_len) * 2;
    new_lss = luoRealloc(lss, LUO_STR_LEN + new_len + 1);

    if (new_lss == NULL) {
        return NULL;
    }

    new_lss->free = new_len - len;
    return new_lss->buf;
}

luo_str
luoStrCatLen(luo_str ls, void *t, long len) {
    struct luo_str_s *lss;

    long current_len = luoStrLen(ls);

    ls = luoStrMakeRoomFor(ls, len);

    if (ls == NULL) {
        return NULL;
    }

    lss = (void *) (ls - LUO_STR_LEN);
    memcpy(ls + current_len, t, len);
    lss->len  = current_len + len;
    lss->free = lss->free - len;
    ls[current_len + len] = '\0';
    return ls;
}

luo_str
luoStrCat(luo_str ls, char *t) {
    return luoStrCatLen(ls, t, strlen(t));
}

luo_str
luoStrCopyLen(luo_str ls, char *t, long len) {
    struct luo_str_s *lss = (void *) (ls - LUO_STR_LEN);

    long total_len = lss->free + lss->len;

    if (total_len < len) {
        ls = luoStrMakeRoomFor(ls, len - total_len);
        if (ls == NULL) {
            return NULL;
        }

        lss = (void *) (ls - LUO_STR_LEN);

        total_len = lss->free + lss->len;
    }

    memcpy(ls, t, len);
    ls[len] = '\0';
    lss->len  = len;
    lss->free = total_len - len;

    return ls;
}

luo_str
luoStrCopy(luo_str ls, char *t) {
    return luoStrCopyLen(ls, t, strlen(t));
}

luo_str
luoStrCatPrintf(luo_str ls, const char *fmt, ...) {
    va_list ap;
    char    *buf, *t;
    size_t  buf_len = 32;

    while (1) {
        buf = luoMalloc(buf_len);

        if (buf == NULL) {
            return NULL;
        }

        buf[buf_len - 2] = '\0';
        va_start(ap, fmt);
        vsnprintf(buf, buf_len, fmt, ap);
        va_end(ap);

        if (buf[buf_len - 2] != '\0') {
            luoFree(buf);
            buf_len *= 2;
            continue;
        }

        break;
    }

    t = luoStrCat(ls, buf);
    luoFree(buf);

    return t;
}

luo_str
luoStrTrim(luo_str ls, const char *c) {
    struct luo_str_s *lss = (void *) (ls - LUO_STR_LEN);

    char   *start, *end, *sp, *ep;
    size_t len;

    sp = start = ls;
    ep = end   = ls + luoStrLen(ls) - 1;

    while (sp <= end && strchr(c, *sp)) {
        sp++;
    }

    while (ep > start && strchr(c, *ep)) {
        ep--;
    }

    len = (sp > ep) ? 0 : ((ep - sp) + 1);

    if (lss->buf != sp) {
        memmove(lss->buf, sp, len);
    }

    lss->buf[len] = '\0';
    lss->free = lss->free + (lss->len - len);
    lss->len  = len;
    return ls;
}

luo_str
luoStrRange(luo_str ls, long start, long end) {
    struct luo_str_s *lss = (void *) (ls - LUO_STR_LEN);

    long new_len;

    long len = luoStrLen(ls);

    if (len == 0) {
        return ls;
    }

    if (start < 0) {
        start += len;
        if (start < 0) {
            start = 0;
        }
    }

    if (end < 0) {
        end += len;
        if (end < 0) {
            end = 0;
        }
    }

    new_len = (start > end) ? 0 : ((end - start) + 1);

    if (new_len != 0) {
        if (start >= (signed) len) {
            start = len - 1;
        }

        if (end >= (signed) len) {
            end = len - 1;
        }

        new_len = (start > end) ? 0 : ((end - start) + 1);
    } else {
        start = 0;
    }

    if (start != 0) {
        memmove(lss->buf, lss->buf + start, new_len);
    }

    lss->buf[new_len] = 0;
    lss->free = lss->free + (lss->len - new_len);
    lss->len  = new_len;
    return ls;
}

void
luoStrToLower(luo_str ls) {
    int i;

    long len = luoStrLen(ls);

    for (i = 0; i < len; ++i) {
        ls[i] = (char) tolower(ls[i]);
    }
}

void
luoStrToUpper(luo_str ls) {
    int i;

    long len = luoStrLen(ls);

    for (i = 0; i < len; ++i) {
        ls[i] = (char) toupper(ls[i]);
    }
}

int
luoStrCmp(luo_str ls1, luo_str ls2) {
    long len1, len2, min_len;
    int  cmp;

    len1 = luoStrLen(ls1);
    len2 = luoStrLen(ls2);

    min_len = (len1 < len2) ? len1 : len2;

    cmp = memcmp(ls1, ls2, (size_t) min_len);

    if (cmp == 0) {
        return (int) (len1 - len2);
    }

    return cmp;
}

luo_str *
luoStrSplitLen(char *s, int len, char *sep, int sep_len, int *count) {
    int elements = 0;
    int slots    = 5;
    int start    = 0;
    int i;

    luo_str *tokens = luoMalloc(sizeof(luo_str) * slots);

    if (sep_len < 1 || len < 0 || tokens == NULL) {
        return NULL;
    }

    for (i = 0; i < (len - (sep_len - 1)); ++i) {
        if (slots < elements + 2) {
            luo_str *new_tokens;

            slots *= 2;

            new_tokens = luoRealloc(tokens, sizeof(luo_str) * slots);

            if (new_tokens == NULL) {
                goto _cleanup;
            }

            tokens = new_tokens;
        }

        if ((sep_len == 1 && *(s + i) == sep[0]) || (memcmp(s + i, sep, (size_t) sep_len) == 0)) {
            tokens[elements] = luoStrCreate(s + start, i - start);

            if (tokens[elements] == NULL) {
                goto _cleanup;
            }

            elements++;

            start = i + sep_len;

            i = i + sep_len - 1;
        }
    }

    tokens[elements] = luoStrCreate(s + start, len - start);

    if (tokens[elements] == NULL) {
        goto _cleanup;
    }

    elements++;
    *count = elements;

    return tokens;

    _cleanup: {
        int j;

        for (j = 0; j < elements; ++j) {
            luoFree(tokens[j]);
        }

        luoFree(tokens);
        return NULL;
    }
}

