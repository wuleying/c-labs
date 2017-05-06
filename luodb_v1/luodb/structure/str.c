//
// _                 _ _             _
// | |_   _  ___   __| | |__   __   _/ |
// | | | | |/ _ \ / _` | '_ \  \ \ / / |
// | | |_| | (_) | (_| | |_) |  \ V /| |
// |_|\__,_|\___/ \__,_|_.__/    \_/ |_|
//
// Created by luoliang1 on 2017/5/4.
//

#include <luodb/structure/str.h>

luo_str
luoStrCreate(const void *init, long len) {
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

luo_str
luoStrNew(const char *init) {
    size_t len = (init == NULL) ? 0 : strlen(init);
    return luoStrCreate(init, len);
}

luo_str
luoEmpty() {
    return luoStrCreate("", 0);
}

long
luoStrLen(const luo_str ls) {
    struct luo_str_s *lss = (void *) (ls - LUO_STR_LEN);
    return lss->len;
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

    long totle_len = lss->free + lss->len;

    if (totle_len < len) {
        ls = luoStrMakeRoomFor(ls, len - totle_len);
        if (ls == NULL) {
            return NULL;
        }

        lss = (void *) (ls - LUO_STR_LEN);

        totle_len = lss->free + lss->len;
    }

    memcpy(ls, t, len);
    ls[len] = '\0';
    lss->len  = len;
    lss->free = totle_len - len;

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

    char *start, *end, *sp, *ep;
    long len;

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
        lss->buf[len] = '\0';
        lss->free = lss->free + (lss->len - len);
        lss->len  = len;
    }

    return ls;
}