//
// Created by luoliang1 on 2017/7/6.
//

#ifndef LUODB_LUODB_CLI_H
#define LUODB_LUODB_CLI_H

#include <luodb/config/config.h>
#include <luodb/structure/str.h>
#include <luodb/util/file.h>
#include <luodb/util/log.h>

static struct luodb_client_config_s {
    char *host_ip;
    int  host_port;
} luodb_client_config;

struct luodb_command_s {
    char *name;
    int  arity;
    int  flags;
};

#endif //LUODB_LUODB_CLI_H
