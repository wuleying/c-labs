//
// Created by luoliang1 on 2017/8/25.
//

#ifndef LUODB_OBJECT_OBJECT_H
#define LUODB_OBJECT_OBJECT_H

#include <assert.h>
#include <stdbool.h>

#include <luodb/header/common.h>
#include <luodb/header/structure.h>

typedef struct luo_object_s {
    void *ptr;
    int  type;
    int  refcount;
} luo_object;

void luoObjectDecrRefcount(void *object, luo_dlist *object_free_list);

#endif //LUODB_OBJECT_H
