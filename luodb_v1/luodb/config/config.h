//
//  _                 _ _             _
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

#include <luodb/common/constant.h>
#include <luodb/net/tcp.h>
#include <luodb/event/event.h>
#include <luodb/structure/str.h>
#include <luodb/structure/dlist.h>
#include <luodb/structure/dict.h>
#include <luodb/util/log.h>
#include <luodb/db/db.h>
#include <luodb/object/object.h>

// 配置文件行最大字节数
#define LUO_CONFIG_LINE_MAX     1024

// 数据库默认数量
#define LUO_DEFAULT_DB_NUMBER   8

/* 结构体定义 */
typedef struct luo_client_s {
    int        fd;
    luo_db     *db;
    int        dict_id;
    luo_str    query_buf;
    luo_object **argv;
    int        argc;
    int        bulk_len;
    luo_dlist  *reply;
    int        sent_len;
    time_t     last_interaction;
    int        flags;
    int        slave_sel_db;
    int        authenticated;
    int        repl_state;
    int        repl_db_fd;
    long       repl_db_off;
    off_t      repl_db_size;
} luo_client;

struct save_param_s {
    time_t seconds;
    int    changes;
};

// 服务端配置
struct luo_server_s {
    int            daemonize;                   // 是否守护进程
    char           *bind_addr;                  // 绑定地址
    int            port;                        // 端口
    char           net_error[LUO_TCP_ERR_LEN];  // 网络错误信息
    int            fd;                          // file descriptor
    char           *pid_file_path;              // pid 文件路径
    char           *log_file_dir;               // 日志文件目录
    int            log_level;                   // 日志级别
    luo_client     *master;
    luo_dlist      *clients;                    // 客户端列表
    luo_dlist      *slaves;
    luo_dlist      *monitors;
    int            repl_state;
    unsigned int   max_clients;                 // 最大客户端数量
    int            cron_loops;
    int            bg_save_inprogress;
    time_t         last_save;
    long long      dirty;
    size_t         used_memory;
    time_t         stat_start_time;             // 服务开始时间
    long long      stat_commands_num;           // 进程命令数量
    long long      stat_connections_num;        // 连接数量
    luo_event_loop *event_loop;                 // 事件循环
    luo_db         *db;                         // 数据库
    int            db_num;                      // 数据库数量
};

struct luo_server_s luo_server;

/* 接口 */
// 初始化服务端配置(使用默认值)
void initServerConfig();

// 加载服务端配置文件
void loadServerConfig(char *file_name);

#endif //LUODB_CONFIG_H
