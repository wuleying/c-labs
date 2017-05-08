//
// _                 _ _             _
// | |_   _  ___   __| | |__   __   _/ |
// | | | | |/ _ \ / _` | '_ \  \ \ / / |
// | | |_| | (_) | (_| | |_) |  \ V /| |
// |_|\__,_|\___/ \__,_|_.__/    \_/ |_|
//
// Created by luoliang1 on 2017/5/5.
//

#include <luodb/config/config.h>

void
initServerConfig() {
    luo_server.port      = LUO_SERVER_TCP_PORT;
    luo_server.log_file  = NULL;
    luo_server.log_level = LUO_LOG_DEUBG;
}

void
loadServerConfig(char *file_name) {
    FILE    *fp      = fopen(file_name, "r");
    char    buf[LUO_CONFIG_LINE_MAX + 1];
    int     line_num = 0;
    char    *err     = NULL;
    luo_str line     = NULL;

    if (!fp) {
        luoLog(LUO_LOG_WARNING, "Fatal error, can't open config file [%s].", file_name);
        exit(1);
    }

    while (fgets(buf, LUO_CONFIG_LINE_MAX + 1, fp) != NULL) {
        luo_str *argv;

        int argc, i;

        line_num++;

        line = luoStrNew(buf);
        line = luoStrTrim(line, " \t\r\n");

        if (line[0] == '#' || line[0] == '\0') {
            luoStrFree(line);
            continue;
        }

        argv = luoStrSplitLen(line, (int) luoStrLen(line), " ", 1, &argc);
        luoStrToLower(argv[0]);

        // 端口
        if (!strcasecmp(argv[0], "port") && argc == 2) {
            luo_server.port = atoi(argv[1]);

            if (luo_server.port < 1 || luo_server.port > 65535) {
                err = "Invalid port";
                goto _displayError;
            }
        }

        // 日志文件路径
        if (!strcasecmp(argv[0], "log_file") && argc == 2) {
            luo_server.log_file = luoStrdup(argv[1]);

            if (!strcasecmp(luo_server.log_file, "stdout")) {
                luoFree(luo_server.log_file);
                luo_server.log_file = NULL;
            }

            if (luo_server.log_file) {
                FILE *log_fp;

                log_fp = fopen(luo_server.log_file, "a");
                if (log_fp == NULL) {
                    err = luoStrCatPrintf(luoStrEmpty(), "Can't open the log file: [%s]", strerror(errno));

                    goto _displayError;
                }
                fclose(log_fp);
            }
        }

        // 日志级别
        if (!strcasecmp(argv[0], "log_level") && argc == 2) {
            if (!strcasecmp(argv[1], "debug")) {
                luo_server.log_level = LUO_LOG_DEUBG;
            } else if (!strcasecmp(argv[1], "notice")) {
                luo_server.log_level = LUO_LOG_NOTICE;
            } else if (!strcasecmp(argv[1], "warning")) {
                luo_server.log_level = LUO_LOG_WARNING;
            } else {
                err = "Invaild log level. Must be one of debug, notice, warning.";
                goto _displayError;
            }
        }
    }

    return;

    _displayError:
    {
        fprintf(stderr, "\n*** FATAL CONFIG FILE ERROR ***\n");
        fprintf(stderr, "Reading the configuration file, at line %d\n", line_num);
        fprintf(stderr, ">>> '%s'\n", line);
        fprintf(stderr, "%s\n", err);
        exit(1);
    }

}