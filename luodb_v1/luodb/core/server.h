//
// _                 _ _             _
// | |_   _  ___   __| | |__   __   _/ |
// | | | | |/ _ \ / _` | '_ \  \ \ / / |
// | | |_| | (_) | (_| | |_) |  \ V /| |
// |_|\__,_|\___/ \__,_|_.__/    \_/ |_|
//
// Created by luoliang1 on 2017/5/6.
//

#ifndef LUODB_SERVER_H
#define LUODB_SERVER_H

#include <signal.h>

#include <luodb/header/common.h>
#include <luodb/header/config.h>
#include <luodb/header/event.h>
#include <luodb/header/memory.h>
#include <luodb/header/net.h>
#include <luodb/header/structure.h>
#include <luodb/header/util.h>

/* 结构体定义 */
typedef struct luo_object_s {
    void *ptr;
    int  type;
    int  refcount;
} luo_object;

typedef struct luo_db_s {
    luo_dict *dict;
    luo_dict *expires;
    int      id;
} luo_db;

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
} luodb_client;

struct save_param_s {
    time_t seconds;
    int    changes;
};

#endif //LUODB_SERVER_H