//
// _                 _ _             _
// | |_   _  ___   __| | |__   __   _/ |
// | | | | |/ _ \ / _` | '_ \  \ \ / / |
// | | |_| | (_) | (_| | |_) |  \ V /| |
// |_|\__,_|\___/ \__,_|_.__/    \_/ |_|
//
// Created by luoliang1 on 2017/5/4.
//

#include <luodb/core/luodb.h>

int main(int argc, char *argv[]) {
    // 初始化服务端配置
    initServerConfig();

    if (argc == 2) {
        loadServerConfig(argv[1]);
    } else if (argc > 2) {
        fprintf(stderr, "Usage: bin/luodb [/path/to/luodb.conf]\n");
        exit(1);
    } else {
        luoLog(LUO_LOG_WARNING, "No config file specified, using the default config.");
    }

    luoLog(LUO_LOG_NOTICE, "Luodb start success, %s %s", argv[0], argv[1]);

    return 0;
}