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
#include <errno.h>

#include <luodb/header/structure.h>
#include <luodb/header/util.h>

// TCP端口
#define LUO_SERVER_TCP_PORT     10088
// 配置文件行最大字节
#define LUO_CONFIG_LINE_MAX     1024

// 服务端配置
struct luo_server_s {
    int port;
    int fd;

    char *log_path;
    int  log_level;
};

struct luo_server_s luo_server;

// 初始化服务端配置(使用默认值)
void initServerConfig();

// 加载服务端配置文件
void loadServerConfig(char *file_name);

#endif //LUODB_CONFIG_H
