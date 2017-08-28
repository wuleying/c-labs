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

/* hash table type implementation */
static int
_luoStrDictKeyCompare(void *priv_data, const void *key1, const void *key2) {
    int i, j;

    LUO_NOT_USED(priv_data);

    i = luoStrLen((luo_str) key1);
    j = luoStrLen((luo_str) key2);

    if (i != j) {
        return 0;
    }

    return memcmp(key1, key2, (size_t) i) == 0;
}

static void
_luoDictObjectDestructor(void *priv_data, void *object) {
    LUO_NOT_USED(priv_data);

    luoObjectDecrRefcount(object);
}

static int
_luoDictStrKeyCompare(void *priv_data, const void *key1, const void *key2) {
    const luo_object *obj1 = key1;
    const luo_object *obj2 = key2;

    return _luoStrDictKeyCompare(priv_data, obj1->ptr, obj2->ptr);
}

static unsigned int
_luoDictStrHash(const void *key) {
    const luo_object *obj = key;

    return luoDictGenHashFunction(obj->ptr, luoStrLen((luo_str) obj->ptr));
}

static
luo_dict_type luoSetDictType = {
        _luoDictStrHash,            // 哈希函数
        NULL,                       // key复制函数
        NULL,                       // val复制函数
        _luoDictStrKeyCompare,      // key比较函数
        _luoDictObjectDestructor,   // key销毁函数
        NULL                        // val销毁函数
};

static
luo_dict_type luoHashDictType = {
        _luoDictStrHash,            // 哈希函数
        NULL,                       // key复制函数
        NULL,                       // val复制函数
        _luoDictStrKeyCompare,      // key比较函数
        _luoDictObjectDestructor,   // key销毁函数
        _luoDictObjectDestructor    // val销毁函数
};

/* private methods */
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
_luoCloseTimeoutClients(void) {
    luo_client     *client;
    luo_dlist_node *dlist_node;
    time_t         now = time(NULL);

    luoDListRewind(luo_server.clients);

    while ((dlist_node = luoDListYield(luo_server.clients)) != NULL) {
        client = LUO_DLIST_NODE_VALUE(dlist_node);

        if (!(client->flags & LUO_CLIENT_SLAVE) &&
            !(client->flags & LUO_CLIENT_MASTER) &&
            (now - client->last_interaction > luo_server.max_idle_time)) {

            _luoFreeClient(client);
        }
    }
}

void
_luoTryResizeHashTables(void) {
    for (int i = 0; i < luo_server.db_num; ++i) {
        long long size = (long long) LUO_DICT_GET_SIZE(luo_server.db[i].dict);
        long long used = (long long) LUO_DICT_GET_USED(luo_server.db[i].dict);

        if (size && used && size > LUO_HASH_TABLE_MIN_SLOTS
            && (used * 100 / size < LUO_HASH_TABLE_MIN_FILL)) {
            luoDictResize(luo_server.db[i].dict);
            luoLogInfo("The hash table %d resized.", i);
        }
    }
}

static int
_luoServerCron(struct luo_event_loop *event_loop, long long id, void *client_data) {
    int i;
    int loops = luo_server.cron_loops++;

    LUO_NOT_USED(event_loop);
    LUO_NOT_USED(id);
    LUO_NOT_USED(client_data);

    // 获取已使用内存
    luo_server.used_memory = luoMallocUsedMemory();

    /*
    for (i = 0; i < luo_server.db_num; ++i) {
        long long size = (long long) LUO_DICT_GET_SIZE(luo_server.db[i].dict);
        long long used = (long long) LUO_DICT_GET_USED(luo_server.db[i].dict);
        long long keys = (long long) LUO_DICT_GET_USED(luo_server.db[i].expires);
    }
    */

    if (!luo_server.bg_save_inprogress) {
        _luoTryResizeHashTables();
    }

    if (luo_server.max_idle_time && !(loops % 10)) {
        // 关闭等待超时的客户端
        _luoCloseTimeoutClients();
    }

    return 1000;
}

static void
_luoInitServer() {
    // 忽略SIGINT信号
    signal(SIGHUP, SIG_IGN);
    // 忽略SIGPIPE信号
    signal(SIGPIPE, SIG_IGN);

    luo_server.clients  = luoDListCreate();
    luo_server.slaves   = luoDListCreate();
    luo_server.monitors = luoDListCreate();

    luo_server.event_loop = luoEventLoopCreate();

    luo_server.db = luoMalloc(sizeof(luo_db) * luo_server.db_num);

    if (!luo_server.clients || !luo_server.slaves || !luo_server.monitors || !luo_server.event_loop || !luo_server.db) {
        luoLogError("Server initialization");
        luoOom("Server initialization");
    }

    luo_server.fd = luoTcpServer(luo_server.net_error, luo_server.port, luo_server.bind_addr);
    luoLogInfo("Opening TCP %s:%d", luo_server.bind_addr, luo_server.port);

    if (luo_server.fd == -1) {
        luoLogError("Opening TCP port, %s", luo_server.net_error);
        exit(1);
    }

    // 创建数据库字典
    for (int i = 0; i < luo_server.db_num; ++i) {
        luo_server.db[i].dict    = luoDictCreate(&luoHashDictType, NULL);
        luo_server.db[i].expires = luoDictCreate(&luoSetDictType, NULL);
        luo_server.db[i].id      = i;
    }

    luo_server.cron_loops           = 0;
    luo_server.bg_save_inprogress   = 0;
    luo_server.last_save            = time(NULL);
    luo_server.dirty                = 0;
    luo_server.used_memory          = 0;
    luo_server.stat_commands_num    = 0;
    luo_server.stat_connections_num = 0;
    luo_server.stat_start_time      = time(NULL);

    luoEventTimeCreate(luo_server.event_loop, 1000, _luoServerCron, NULL, NULL);
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

    if (luo_server.max_clients && LUO_DLIST_LENGTH(luo_server.clients) > luo_server.max_clients) {
        char *err = "-ERR max number of clients reached\r\n";
        (void) write(client->fd, err, strlen(err));
        _luoFreeClient(client);
        return;
    }

    luo_server.stat_connections_num++;
}

// 主函数
int main(int argc, char *argv[]) {
    // 初始化服务端配置
    initServerConfig();

    if (argc == 2) {
        // 加载指定的配置文件
        loadServerConfig(argv[1]);
    } else if (argc > 2) {
        luoLogError("Usage: bin/luodb [/path/to/luodb.conf]");
        exit(1);
    } else {
        // 使用默认配置项
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