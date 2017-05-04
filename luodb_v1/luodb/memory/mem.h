//
// Created by luoliang1 on 2017/5/4.
//

#ifndef LUODB_MEM_H
#define LUODB_MEM_H

#include <stdlib.h>
#include <string.h>

void *luo_malloc(size_t size);

void *luo_realloc(void *ptr, size_t size);

void luo_free(void *ptr);

char *luo_strdup(const char *s);

size_t luo_malloc_used_memory(void);

#endif //LUODB_MEM_H