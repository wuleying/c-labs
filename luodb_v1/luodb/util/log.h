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

// 日志级别
#define LUO_LOG_DEUBG           0
#define LUO_LOG_NOTICE          1
#define LUO_LOG_WARNING         2
#define LUO_LOG_ERROR           3

static char *log_level_array[] = {"Debug", "Notice", "Warning", "Error"};

// 调试级别日志
void luoLogDebug(const char *fmt, ...);

// 注意级别日志
void luoLogNotice(const char *fmt, ...);

// 警告级别日志
void luoLogWarning(const char *fmt, ...);

// 错误级别日志
void luoLogError(const char *fmt, ...);

#endif //LUODB_UTIL_LOG_H
