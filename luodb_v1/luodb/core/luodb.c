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
_initServer() {
    int j;

    // 忽略SIGINT信号
    signal(SIGHUP, SIG_IGN);
    // 忽略SIGPIPE信号
    signal(SIGPIPE, SIG_IGN);
}

static void
_daemonize(void) {
    int  fd;
    FILE *fp;

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

    fp = fopen(luo_server.pid_file_path, "w");

    if (fp) {
        fprintf(fp, "%d\n", getpid());
        fclose(fp);
    }
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
        luoLog(LUO_LOG_DEUBG, "Accepting client connection: %s", luo_server.net_error);
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

    // 保存pid
    pid_t pid = getpid();
    luoFileSavePid(luo_server.pid_file_path, pid);

    luoLog(LUO_LOG_DEUBG, "Luodb start success. VERSION:%s, PID:%d, CLI:%s %s", LUODB_VERSION, pid, argv[0], argv[1]);

    luo_event_loop *event_loop = luoEventLoopCreate();

    luoEventFileCreate(event_loop, luo_server.fd, LUO_EVENT_READABLE, _luoAcceptHandler, NULL, NULL);

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