//
// Created by luoliang1 on 2017/7/6.
//

#ifndef LUODB_LUODB_CLI_H
#define LUODB_LUODB_CLI_H

#include <luodb/config/config.h>
#include <luodb/structure/str.h>
#include <luodb/util/file.h>
#include <luodb/util/log.h>

#define LUO_CMD_INLINE            1
#define LUO_CMD_BULK              2

#define LUO_CLIENT_OK             0
#define LUO_CLIENT_ERR            1

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

#endif //LUODB_LUODB_CLI_H