//
// _                 _ _             _
// | |_   _  ___   __| | |__   __   _/ |
// | | | | |/ _ \ / _` | '_ \  \ \ / / |
// | | |_| | (_) | (_| | |_) |  \ V /| |
// |_|\__,_|\___/ \__,_|_.__/    \_/ |_|
//
// Created by luoliang1 on 2017/5/4.
//

#include <luodb/core/luodb-server.h>

static void
_initServer() {
    // 忽略SIGINT信号
    signal(SIGHUP, SIG_IGN);
    // 忽略SIGPIPE信号
    signal(SIGPIPE, SIG_IGN);

    luo_server.event_loop = luoEventLoopCreate();

    luo_server.fd = luoTcpServer(luo_server.net_error, luo_server.port, luo_server.bind_addr);

    luoLog(LUO_LOG_DEUBG, "Opening TCP %s:%d", luo_server.bind_addr, luo_server.port);

    if (luo_server.fd == -1) {
        luoLog(LUO_LOG_WARNING, "Opening TCP port: %s", luo_server.net_error);
        exit(1);
    }
}

static void
_daemonize(void) {
    int fd;

    if (fork() != 0) {
        exit(0);
    }

    // 创建一个新会话
    setsid();

    if ((fd = open("/dev/null", O_RDWR, 0)) != -1) {
        dup2(fd, STDIN_FILENO);
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);

        if (fd > STDERR_FILENO) {
            close(fd);
        }
    }

    luoFileSavePid(luo_server.pid_file_path, getpid());
}

static void
_luoAcceptHandler(luo_event_loop *event_loop, int fd, void *priv_data, int mask) {
    int  client_port;
    int  client_fd;
    char client_ip[128];

    LUODB_NOT_USED(event_loop);
    LUODB_NOT_USED(priv_data);
    LUODB_NOT_USED(mask);

    client_fd = luoTcpAccept(luo_server.net_error, fd, client_ip, &client_port);

    if (client_fd == LUO_TCP_ERR) {
        return;
    }

    luoLog(LUO_LOG_DEUBG, "Accepted %s:%d", client_ip, client_port);
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

    // 初始化服务端
    _initServer();

    // 以守护进程运行
    if (luo_server.daemonize) {
        _daemonize();
    }

    luoLog(LUO_LOG_DEUBG, "Luodb start success. VERSION:%s, CLI:%s %s", LUODB_VERSION, argv[0], argv[1]);

    if (luoEventFileCreate(luo_server.event_loop, luo_server.fd, LUO_EVENT_READABLE, _luoAcceptHandler, NULL, NULL) ==
        LUO_EVENT_ERR) {
        fprintf(stderr, "Create file event: Out of memory\n");
        fflush(stderr);
        sleep(1);
        abort();
    }

    luoEventMain(luo_server.event_loop);

    luoEventLoopDelete(luo_server.event_loop);

    return 0;
}