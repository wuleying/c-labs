//
// Created by luoliang1 on 2017/5/4.
//

#ifndef LUODB_MEM_H
#define LUODB_MEM_H

#include <stdlib.h>
#include <string.h>

void *luoMalloc(size_t size);

void *luoRealloc(void *ptr, size_t size);

void luoFree(void *ptr);

char *luoStrdup(const char *s);

size_t luoMallocUsedMemory(void);

#endif //LUODB_MEM_H