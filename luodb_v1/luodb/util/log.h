//
// _                 _ _             _
// | |_   _  ___   __| | |__   __   _/ |
// | | | | |/ _ \ / _` | '_ \  \ \ / / |
// | | |_| | (_) | (_| | |_) |  \ V /| |
// |_|\__,_|\___/ \__,_|_.__/    \_/ |_|
//
// Created by luoliang1 on 2017/5/5.
//

#ifndef LUODB_UTIL_LOG_H
#define LUODB_UTIL_LOG_H

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#include <luodb/config/config.h>
#include <luodb/util/file.h>

void luoLog(int level, const char *fmt, ...);

#endif //LUODB_UTIL_LOG_H
