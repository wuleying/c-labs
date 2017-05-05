//
// Created by luoliang1 on 2017/5/5.
//

#ifndef LUODB_LOG_H
#define LUODB_LOG_H

#include <stdarg.h>
#include <stdio.h>
#include <time.h>

#include <luodb/config/config.h>

void luoLog(int level, const char *fmt, ...);

#endif //LUODB_LOG_H
