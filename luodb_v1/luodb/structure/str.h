//
// Created by luoliang1 on 2017/5/4.
//

#ifndef LUODB_STRING_H
#define LUODB_STRING_H

#include <string.h>
#include <sys/types.h>

#include <luodb/memory/mem.h>

typedef char *luo_str_t;

struct luo_str {
    long len;
    long free;
    char buf[];
};

luo_str_t luo_str_create(const void *init, size_t len);

#endif //LUODB_STRING_H