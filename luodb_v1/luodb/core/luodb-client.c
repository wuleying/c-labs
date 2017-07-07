//
// Created by luoliang1 on 2017/7/6.
//

#include <luodb/core/luodb-client.h>

static int
_luodbClientParseOptions(int argc, char **argv) {
    int i;

    for (i = 1; i < argc; ++i) {
        int last_arg = (i == argc - 1);

        if (!strcmp(argv[i], "-h") && !last_arg) {
            char *ip = luoMalloc(32);

            if (luoTcpResolve(NULL, argv[i + 1], ip) == LUO_TCP_ERR) {
                printf("Can't resolve %s\n", argv[i]);
                exit(1);
            }

            luodb_client_config.host_ip = ip;
        } else if (!strcmp(argv[i], "-p") && !last_arg) {
            luodb_client_config.host_port = atoi(argv[i + 1]);
        } else {
            break;
        }
    }

    return i;
}

int
main(int argc, char **argv) {
    int  first_arg;
    int  i;
    char **argv_copy;

    struct luodb_command_s *luodb_command;

    // 初始化客户端配置
    luodb_client_config.host_ip   = luoStrNew(LUO_SERVER_TCP_IP);
    luodb_client_config.host_port = LUO_SERVER_TCP_PORT;

    first_arg = _luodbClientParseOptions(argc, argv);
    argc -= first_arg;
    argv += first_arg;

    luoLog(LUO_LOG_DEUBG, "first_arg = %d, avgc = %d, argv = %d", first_arg, argc, argv);

    argv_copy = luoMalloc(sizeof(char *) * argc + 1);

    for (i = 0; i < argc; ++i) {
        argv_copy[i] = luoStrNew(argv[i]);
    }

    if (argc < 1) {
        fprintf(stderr, "Usage: luodb-client [-h host] [-p port] cmd arg1 arg2 arg3 ... argN\n");
        exit(1);
    }

    luoLog(LUO_LOG_DEUBG, "host_ip = %s, host_port = %d", luodb_client_config.host_ip, luodb_client_config.host_port);

    return 0;
}