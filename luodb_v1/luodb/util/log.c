//
// _                 _ _             _
// | |_   _  ___   __| | |__   __   _/ |
// | | | | |/ _ \ / _` | '_ \  \ \ / / |
// | | |_| | (_) | (_| | |_) |  \ V /| |
// |_|\__,_|\___/ \__,_|_.__/    \_/ |_|
//
// Created by luoliang1 on 2017/5/5.
//

#include <luodb/util/log.h>

// 私有方法,日志落地
static void
_luoLog(int level, const char *fmt, va_list ap) {
    // 日志级别低于设置的最低日志级别,直接返回
    if (level < luo_server.log_level) {
        return;
    }

    // 文件操作句柄
    FILE   *fp;
    // 当前时间
    time_t now = time(NULL);

    if (luo_server.log_file_dir == NULL) {
        // 未设置日志目录,直接将日志输出到标准输出
        fp = stdout;
    } else {
        luo_str date_buf     = luoStrCreate("", 64);
        luo_str log_file_dir = luoStrCreate("", 1024);
        luo_str log_file     = luoStrCreate("", 1024);

        strftime(date_buf, 64, "%G/%m/%d", localtime(&now));

        sprintf(log_file_dir, "%s/%s", luo_server.log_file_dir, date_buf);

        // 创建目录
        luoFileMakeDirs(log_file_dir);

        sprintf(log_file, "%s/luodb.log", log_file_dir);
        fp = fopen(log_file, "a");

        luoStrFree(date_buf);
        luoStrFree(log_file_dir);
        luoStrFree(log_file);
    }

    if (fp == NULL) {
        return;
    }

    luo_str buf = luoStrCreate("", 64);

    strftime(buf, 64, "%F %H:%M:%S", localtime(&now));
    fprintf(fp, "%s [%5s] ", buf, log_level_array[level]);
    vfprintf(fp, fmt, ap);
    fprintf(fp, "\n");
    fflush(fp);

    luoStrFree(buf);
    va_end(ap);

    if (luo_server.log_file_dir) {
        fclose(fp);
    }
}

// 通用日志
void
luoLog(int level, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    _luoLog(level, fmt, ap);
    va_end(ap);
}

// 追踪日志
void
luoLogTrace(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    _luoLog(LUO_LOG_TRACE, fmt, ap);
    va_end(ap);
}

// 调试级别日志
void
luoLogDebug(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    _luoLog(LUO_LOG_DEBUG, fmt, ap);
    va_end(ap);
}

// 常规信息级别日志
void
luoLogInfo(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    _luoLog(LUO_LOG_INFO, fmt, ap);
    va_end(ap);
}

// 警告级别日志
void
luoLogWarn(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    _luoLog(LUO_LOG_WARN, fmt, ap);
    va_end(ap);
}

// 错误级别日志
void
luoLogError(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    _luoLog(LUO_LOG_ERROR, fmt, ap);
    va_end(ap);
}

// 致命错误级别日志
void
luoLogFatal(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    _luoLog(LUO_LOG_FATAL, fmt, ap);
    va_end(ap);
}