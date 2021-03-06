//
//  _                 _ _             _
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
#define LUO_LOG_TRACE           0
#define LUO_LOG_DEBUG           1
#define LUO_LOG_INFO            2
#define LUO_LOG_WARN            3
#define LUO_LOG_ERROR           4
#define LUO_LOG_FATAL           5

static char *log_level_array[] = {"TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"};

// 通用日志
void luoLog(int level, const char *fmt, ...);

// 追踪级别日志
void luoLogTrace(const char *fmt, ...);

// 调试级别日志
void luoLogDebug(const char *fmt, ...);

// 注意级别日志
void luoLogInfo(const char *fmt, ...);

// 警告级别日志
void luoLogWarn(const char *fmt, ...);

// 错误级别日志
void luoLogError(const char *fmt, ...);

// 致命错误级别日志
void luoLogFatal(const char *fmt, ...);

#endif //LUODB_UTIL_LOG_H
