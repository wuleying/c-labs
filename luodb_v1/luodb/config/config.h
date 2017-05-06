//
// _                 _ _             _
// | |_   _  ___   __| | |__   __   _/ |
// | | | | |/ _ \ / _` | '_ \  \ \ / / |
// | | |_| | (_) | (_| | |_) |  \ V /| |
// |_|\__,_|\___/ \__,_|_.__/    \_/ |_|
//
// Created by luoliang1 on 2017/5/5.
//

#ifndef LUODB_CONFIG_H
#define LUODB_CONFIG_H

#include <stdlib.h>
#include <stdio.h>

#include <luodb/header/structure.h>
#include <luodb/header/util.h>

// TCP端口
#define LUO_SERVER_TCP_PORT     10088
// 文件描述符
#define LUO_SERVER_FD           0
// 配置文件行最大字节
#define LUO_CONFIG_LINE_MAX     1024

// 日志级别
#define LUO_LOG_DEUBG           0
#define LUO_LOG_NOTICE          1
#define LUO_LOG_WARNING         2

struct luo_server_s {
    int port;
    int fd;

    char *log_file;
    int  log_level;
};

struct luo_server_s luo_server;

// 初始化服务端配置(使用默认值)
void initServerConfig();

// 加载服务端配置文件
void loadServerConfig(char *file_name);

#endif //LUODB_CONFIG_H
