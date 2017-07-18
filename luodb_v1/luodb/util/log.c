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

static void
_luoLog(int level, const char *fmt, va_list ap) {
    if (level < luo_server.log_level) {
        return;
    }

    FILE   *fp;
    time_t now = time(NULL);

    if (luo_server.log_file_dir == NULL) {
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
    fprintf(fp, "%s [%s] ", buf, log_level_array[level]);
    vfprintf(fp, fmt, ap);
    fprintf(fp, "\n");
    fflush(fp);

    luoStrFree(buf);
    va_end(ap);

    if (luo_server.log_file_dir) {
        fclose(fp);
    }
}

void
luoLog(int level, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    _luoLog(level, fmt, ap);
    va_end(ap);
}

void
luoLogDebug(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    _luoLog(LUO_LOG_DEUBG, fmt, ap);
    va_end(ap);
}

void
luoLogNotice(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    _luoLog(LUO_LOG_NOTICE, fmt, ap);
    va_end(ap);
}

void
luoLogWarning(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    _luoLog(LUO_LOG_WARNING, fmt, ap);
    va_end(ap);
}

void
luoLogError(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    _luoLog(LUO_LOG_ERROR, fmt, ap);
    va_end(ap);
}