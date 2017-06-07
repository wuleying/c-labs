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
#include <luodb/header/net.h>
#include <luodb/event/event.h>

// TCP端口
#define LUO_SERVER_TCP_PORT     10088
// 配置文件行最大字节
#define LUO_CONFIG_LINE_MAX     1024

// 服务端配置
struct luo_server_s {
    int            daemonize;                     // 是否守护进程
    int            port;                          // 端口
    char           net_error[LUO_TCP_ERR_LEN];    // 网络错误信息
    int            fd;                            // file descriptor
    char           *pid_file_path;                // pid 文件路径
    char           *log_file_dir;                 // 日志文件目录
    int            log_level;                     // 日志级别
    luo_event_loop *event_loop;                   // 事件循环
};

struct luo_server_s luo_server;

/* 接口 */
// 初始化服务端配置(使用默认值)
void initServerConfig();

// 加载服务端配置文件
void loadServerConfig(char *file_name);

#endif //LUODB_CONFIG_H
