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

void luoLog(int level, const char *fmt, ...) {
    va_list ap;
    FILE    *fp;

    if (luo_server.log_file == NULL) {
        fp = stdout;
    } else {
        fp = fopen(luo_server.log_file, "a");
    }

    if (fp == NULL) {
        return;
    }

    va_start(ap, fmt);
    if (level >= luo_server.log_level) {
        char   *c = ".-*";
        char   buf[64];
        time_t now;

        now = time(NULL);
        strftime(buf, 64, "%d %b %H:%M:%S", gmtime(&now));
        fprintf(fp, "%s %c ", buf, c[level]);
        vfprintf(fp, fmt, ap);
        fprintf(fp, "\n");
        fflush(fp);
    }
    va_end(ap);

    if (luo_server.log_file) {
        fclose(fp);
    }
}