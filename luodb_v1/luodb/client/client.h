//
//  _                 _ _             _
// | |_   _  ___   __| | |__   __   _/ |
// | | | | |/ _ \ / _` | '_ \  \ \ / / |
// | | |_| | (_) | (_| | |_) |  \ V /| |
// |_|\__,_|\___/ \__,_|_.__/    \_/ |_|
//
// Created by luoliang1 on 2017/7/6.
//

#ifndef LUODB_CLI_H
#define LUODB_CLI_H

#include <luodb/header/common.h>
#include <luodb/header/config.h>
#include <luodb/header/structure.h>
#include <luodb/header/util.h>

static struct luo_client_config_s {
    char *host_ip;
    int  host_port;
} luo_client_config;

struct luo_command_s {
    char *name;
    int  arity;
    int  flags;
};

static struct luo_command_s cmd_table[] = {
        {"get", 2, LUO_CMD_INLINE},
        {"set", 3, LUO_CMD_BULK},
        {NULL,  0, 0}
};

#endif //LUODB_CLI_H