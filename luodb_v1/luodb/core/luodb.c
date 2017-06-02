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

static void
luoAcceptHandler(luo_event_loop *event_loop, int fd, void *priv_data, int mask) {
    int  client_port;
    int  client_fd;
    char client_ip[128];
}


int main(int argc, char *argv[]) {
    // 初始化服务端配置
    initServerConfig();

    if (argc == 2) {
        loadServerConfig(argv[1]);
    } else if (argc > 2) {
        luoLog(LUO_LOG_ERROR, "Usage: bin/luodb [/path/to/luodb.conf]");
        exit(1);
    } else {
        luoLog(LUO_LOG_WARNING, "No config file specified, using the default config.");
    }

    // 保存pid
    pid_t pid = getpid();
    luoFileSavePid(luo_server.pid_file_path, pid);

    luoLog(LUO_LOG_NOTICE, "Luodb start success. VERSION:%s, PID:%d, CLI:%s %s", LUODB_VERSION, pid, argv[0], argv[1]);

    luo_event_loop *event_loop = luoEventLoopCreate();

    luoEventFileCreate(event_loop, luo_server.fd, LUO_EVENT_READABLE, luoAcceptHandler, NULL, NULL);

    /*
    char *ser_err = NULL;
    luo_server.fd = luoTcpServer(ser_err, luo_server.port, NULL);

    luoLog(LUO_LOG_NOTICE, "luo_server.fd = %d", luo_server.fd);

    struct sockaddr_in sa;
    char *ip = luoStrCreate("127.0.0.1", sizeof(sa) + 1);

    int cfd = luoTcpAccept(ser_err, luo_server.fd, ip, &luo_server.port);

    if(cfd == -1) {
        fprintf(stderr, "Tcp accept failed\n");
        exit(1);
    }
    */

    return 0;
}