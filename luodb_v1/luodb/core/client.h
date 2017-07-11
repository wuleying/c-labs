//
// Created by luoliang1 on 2017/7/6.
//

#ifndef LUODB_CLIENT_H
#define LUODB_CLIENT_H

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

#endif //LUODB_CLIENT_H