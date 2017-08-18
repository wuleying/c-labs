//
//  _                 _ _             _
// | |_   _  ___   __| | |__   __   _/ |
// | | | | |/ _ \ / _` | '_ \  \ \ / / |
// | | |_| | (_) | (_| | |_) |  \ V /| |
// |_|\__,_|\___/ \__,_|_.__/    \_/ |_|
//
// Created by luoliang1 on 2017/5/4.
//

#ifndef LUODB_STRING_H
#define LUODB_STRING_H

#include <string.h>
#include <sys/types.h>
#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>

#include <luodb/header/memory.h>

/* 结构体 */
// 声明luo_str为一种char类型
typedef char *luo_str;

// 字符串结构体
struct luo_str_s {
    // 字符串长度
    long len;
    // 可用空间
    long free;
    // 存放具体字符串的buffer
    char buf[];
};

/* 宏 */
// 字符串长度
#define LUO_STR_LEN sizeof(struct luo_str_s)

/* 接口 */
luo_str luoStrCreate(const void *init, long len);

luo_str luoStrNew(const char *init);

luo_str luoStrEmpty();

int luoStrLen(const luo_str ls);

luo_str luoStrDup(const luo_str ls);

void luoStrFree(luo_str ls);

long luoStrAvail(luo_str ls);

static luo_str luoStrMakeRoomFor(luo_str ls, long add_len);

luo_str luoStrCatLen(luo_str ls, void *t, long len);

luo_str luoStrCat(luo_str ls, char *t);

luo_str luoStrCopyLen(luo_str ls, char *t, long len);

luo_str luoStrCopy(luo_str ls, char *t);

luo_str luoStrCatPrintf(luo_str ls, const char *fmt, ...);

luo_str luoStrTrim(luo_str ls, const char *c);

void luoStrToLower(luo_str ls);

void luoStrToUpper(luo_str ls);

int luoStrCmp(luo_str ls1, luo_str ls2);

luo_str *luoStrSplitLen(char *s, int len, char *sep, int sep_len, int *count);

#endif //LUODB_STRING_H