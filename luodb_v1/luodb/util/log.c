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

void
luoLog(int level, const char *fmt, ...) {
    va_list ap;
    FILE    *fp;
    char    date_buf[64];
    char    log_path[1024];
    char    log_file[1024];
    time_t  now = time(NULL);

    if (luo_server.log_path == NULL) {
        fp = stdout;
    } else {
        strftime(date_buf, 64, "%G/%m", gmtime(&now));

        sprintf(log_path, "%s/%s", luo_server.log_path, date_buf);

        luoMakeDirs(log_path);

        sprintf(log_file, "%s/luodb.log", log_path);
        fp = fopen(log_file, "a");
    }

    if (fp == NULL) {
        return;
    }

    va_start(ap, fmt);
    if (level >= luo_server.log_level) {
        char buf[64];

        strftime(buf, 64, "%F %H:%M:%S", gmtime(&now));
        fprintf(fp, "%s [%s] ", buf, log_level_array[level]);
        vfprintf(fp, fmt, ap);
        fprintf(fp, "\n");
        fflush(fp);
    }
    va_end(ap);

    if (luo_server.log_path) {
        fclose(fp);
    }
}