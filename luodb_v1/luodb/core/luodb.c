//
// _                 _ _
// | |_   _  ___   __| | |__
// | | | | |/ _ \ / _` | '_ \
// | | |_| | (_) | (_| | |_) |
// |_|\__,_|\___/ \__,_|_.__/
//
// Created by luoliang1 on 2017/5/4.
//

#include <luodb/header/config.h>
#include <luodb/header/event.h>
#include <luodb/header/memory.h>
#include <luodb/header/net.h>
#include <luodb/header/structure.h>
#include <luodb/header/util.h>

int main(int argc, char *argv[]) {
    // 初始化服务端配置
    initServerConfig();

    if (argc == 2) {
        printf("%s", argv[1]);
    } else if (argc > 2) {
        fprintf(stderr, "Usage: bin/luodb [/path/to/luodb.conf]\n");
        exit(1);
    } else {
        luoLog(LUO_LOG_WARNING, "Warning: no config file specified, using the default config.");
    }

    return 0;
}