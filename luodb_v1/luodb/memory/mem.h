//
//  _                 _ _             _
// | |_   _  ___   __| | |__   __   _/ |
// | | | | |/ _ \ / _` | '_ \  \ \ / / |
// | | |_| | (_) | (_| | |_) |  \ V /| |
// |_|\__,_|\___/ \__,_|_.__/    \_/ |_|
//
// Created by luoliang1 on 2017/5/4.
//

#ifndef LUODB_MEM_H
#define LUODB_MEM_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

void *luoMalloc(size_t size);

void *luoRealloc(void *ptr, size_t size);

void luoFree(void *ptr);

char *luoStrdup(const char *s);

void luoOom(const char *msg);

size_t luoMallocUsedMemory(void);

#endif //LUODB_MEM_H