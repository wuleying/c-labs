//
// Created by luoliang1 on 2017/5/5.
//

#ifndef LUODB_CONFIG_H
#define LUODB_CONFIG_H

#define LUO_SERVER_PORT         10088
#define LUO_SERVER_FD           0

// 日志级别
#define LUO_LOG_DEUBG           0
#define LUO_LOG_NOTICE          1
#define LUO_LOG_WARNING         2

#include <stdlib.h>

struct luo_server_s {
    int port;
    int fd;

    char *log_file;
    int log_level;
};

struct luo_server_s luo_server;

void initServerConfig();

#endif //LUODB_CONFIG_H
