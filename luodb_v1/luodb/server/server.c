//
//  _                 _ _             _
// | |_   _  ___   __| | |__   __   _/ |
// | | | | |/ _ \ / _` | '_ \  \ \ / / |
// | | |_| | (_) | (_| | |_) |  \ V /| |
// |_|\__,_|\___/ \__,_|_.__/    \_/ |_|
//
// Created by luoliang1 on 2017/5/4.
//

#include <luodb/server/server.h>

static void
_luoInitServer() {
    // 忽略SIGINT信号
    signal(SIGHUP, SIG_IGN);
    // 忽略SIGPIPE信号
    signal(SIGPIPE, SIG_IGN);

    luo_server.event_loop = luoEventLoopCreate();

    luo_server.fd = luoTcpServer(luo_server.net_error, luo_server.port, luo_server.bind_addr);

    luoLogInfo("Opening TCP %s:%d", luo_server.bind_addr, luo_server.port);

    if (luo_server.fd == -1) {
        luoLogError("Opening TCP port, %s", luo_server.net_error);
        exit(1);
    }
}

static void
_luoDaemonize(void) {
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

static luo_client *
_luoCreateClient(int fd) {
    luo_client *client = luoMalloc(sizeof(luo_client));

    luoTcpNonBlock(NULL, fd);
    luoTcpNoDelay(NULL, fd);

    if (!client) {
        return NULL;
    }

    // todo selectDB(client, 0);

    client->fd               = fd;
    client->query_buf        = luoStrEmpty();
    client->argc             = 0;
    client->argv             = NULL;
    client->bulk_len         = 0;
    client->sent_len         = 0;
    client->flags            = 0;
    client->last_interaction = time(NULL);
    client->authenticated    = 0;
    client->repl_state       = LUO_REPL_NONE;
    client->reply            = luoDListCreate();

    if (client->reply == NULL) {
        luoOom("List create");
    }

    // todo set free/dup mothod

    return client;
}

static void
_luoFreeClient(luo_client *client) {
    luo_dlist_node *dlist_node;

    luoEventFileDelete(luo_server.event_loop, client->fd, LUO_EVENT_READABLE);
    luoEventFileDelete(luo_server.event_loop, client->fd, LUO_EVENT_WRITABLE);

    luoStrFree(client->query_buf);
    luoDListRelease(client->reply);
    close(client->fd);

    dlist_node = luoDListSearchKey(luo_server.clients, client);
    assert(dlist_node != NULL);
    luoDListDeleteNode(luo_server.clients, dlist_node);

    if (client->flags & LUO_CLIENT_SLAVE) {
        if (client->repl_state == LUO_REPL_SEND_BULK && client->repl_db_fd != -1) {
            close(client->repl_db_fd);
        }

        luo_dlist *dlist = (client->flags & LUO_CLIENT_MONITOR) ? luo_server.monitors : luo_server.slaves;

        dlist_node = luoDListSearchKey(dlist, client);
        luoDListDeleteNode(dlist, dlist_node);
    }

    if (client->flags & LUO_CLIENT_MASTER) {
        luo_server.master     = NULL;
        luo_server.repl_state = LUO_REPL_CONNECT;
    }

    luoFree(client->argv);
    luoFree(client);
}

static void
_luoAcceptHandler(luo_event_loop *event_loop, int fd, void *priv_data, int mask) {
    int  client_port;
    int  client_fd;
    char client_ip[128];

    luo_client *client;

    LUO_NOT_USED(event_loop);
    LUO_NOT_USED(priv_data);
    LUO_NOT_USED(mask);

    client_fd = luoTcpAccept(luo_server.net_error, fd, client_ip, &client_port);

    if (client_fd == LUO_TCP_ERR) {
        return;
    }

    luoLogTrace("Accepted %s:%d", client_ip, client_port);

    client = _luoCreateClient(client_fd);

    if (client == NULL) {
        luoLogWarn("Error allocating resoures for the client.");
        close(client_fd);
        return;
    }

    luoLogTrace("Client number = %d", LUO_DLIST_LENGTH(luo_server.clients));

    if (luo_server.max_clients && LUO_DLIST_LENGTH(luo_server.clients) > luo_server.max_clients) {
        char *err = "-ERR max number of clients reached\r\n";
        (void) write(client->fd, err, strlen(err));
        _luoFreeClient(client);
        return;
    }

    luo_server.stat_connections_num++;
}

int main(int argc, char *argv[]) {
    // 初始化服务端配置
    initServerConfig();

    if (argc == 2) {
        loadServerConfig(argv[1]);
    } else if (argc > 2) {
        luoLogError("Usage: bin/luodb [/path/to/luodb.conf]");
        exit(1);
    } else {
        luoLogWarn("No config file specified, using the default config.");
    }

    // 初始化服务端
    _luoInitServer();

    // 以守护进程运行
    if (luo_server.daemonize) {
        _luoDaemonize();
    }

    luoLogInfo("Luodb start success. Version: %s, Command: %s %s", LUO_VERSION, argv[0], argv[1]);

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