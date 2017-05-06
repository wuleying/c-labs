//
// _                 _ _             _
// | |_   _  ___   __| | |__   __   _/ |
// | | | | |/ _ \ / _` | '_ \  \ \ / / |
// | | |_| | (_) | (_| | |_) |  \ V /| |
// |_|\__,_|\___/ \__,_|_.__/    \_/ |_|
//
// Created by luoliang1 on 2017/5/5.
//

#include <luodb/config/config.h>

void
initServerConfig() {
    luo_server.port      = LUO_SERVER_TCP_PORT;
    luo_server.fd        = LUO_SERVER_FD;
    luo_server.log_file  = NULL;
    luo_server.log_level = LUO_LOG_DEUBG;
}

void
loadServerConfig(char *file_name) {
    FILE    *fp      = fopen(file_name, "r");
    char    buf[LUO_CONFIG_LINE_MAX + 1];
    int     line_num = 0;
    char    *err     = NULL;
    luo_str line     = NULL;

    if(!fp) {
        luoLog(LUO_LOG_WARNING, "Fatal error, can't open config file [%s].", file_name);
        exit(1);
    }

    while (fgets(buf, LUO_CONFIG_LINE_MAX + 1, fp) != NULL) {
        luo_str *argv;
        int argc, i;

        line_num++;

    }
}