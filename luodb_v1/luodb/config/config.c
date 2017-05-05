//
// Created by luoliang1 on 2017/5/5.
//

#include <luodb/config/config.h>

void
initServerConfig() {
    luo_server.port      = LUO_SERVER_PORT;
    luo_server.fd        = LUO_SERVER_FD;
    luo_server.log_file  = NULL;
    luo_server.log_level = LUO_LOG_DEUBG;
}