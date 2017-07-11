//
// Created by luoliang1 on 2017/7/6.
//

#include <luodb/core/luodb-client.h>

static int
_luoClientParseOptions(int argc, char **argv) {
    int i;

    for (i = 1; i < argc; ++i) {
        int last_arg = (i == argc - 1);

        if (!strcmp(argv[i], "-h") && !last_arg) {
            char *ip = luoMalloc(32);

            if (luoTcpResolve(NULL, argv[i + 1], ip) == LUO_TCP_ERR) {
                luoLog(LUO_LOG_ERROR, "Can't resolve %s\n", argv[i]);
                exit(1);
            }

            luo_client_config.host_ip = ip;
            i++;
        } else if (!strcmp(argv[i], "-p") && !last_arg) {
            luo_client_config.host_port = atoi(argv[i + 1]);
            i++;
        } else {
            break;
        }
    }

    return i;
}

static struct luo_command_s *
_luoClientLookupCommand(char *name) {
    int i = 0;

    while (cmd_table[i].name != NULL) {
        if (!strcasecmp(name, cmd_table[i].name)) {
            return &cmd_table[i];
        }
        i++;
    }

    return NULL;
}

static luo_str
_luoClientReadArgFromStdin(void) {
    size_t buf_size = 1024;
    char   buf[buf_size];

    luo_str arg = luoStrEmpty();

    while (1) {
        int nread = (int) read(fileno(stdin), buf, buf_size);

        if (nread == 0) {
            break;
        } else if (nread == -1) {
            luoLog(LUO_LOG_ERROR, "Reading from standard input");
            exit(1);
        }

        arg = luoStrCatLen(arg, buf, nread);
    }

    return arg;
}

static int
_luoClientConnect() {
    int  fd;
    char err[LUO_TCP_ERR_LEN];

    fd = luoTcpConnect(err, luo_client_config.host_ip, luo_client_config.host_port);

    luoLog(LUO_LOG_DEUBG, "host_ip = %s, host_port = %d", luo_client_config.host_ip, luo_client_config.host_port);

    if (fd == LUO_TCP_ERR) {
        luoLog(LUO_LOG_ERROR, "%s", err);
        return -1;
    }

    luoTcpNoDelay(NULL, fd);

    return fd;
}

static luo_str
_luoClientReadLine(int fd) {
    luo_str line = luoStrEmpty();

    while (1) {
        char    c;
        ssize_t result;
        result = read(fd, &c, 1);

        if (result == -1) {
            luoStrFree(line);
            return NULL;
        } else if ((result == 0) || (c == '\n')) {
            break;
        } else {
            line = luoStrCatLen(line, &c, 1);
        }
    }

    return luoStrTrim(line, "\r\n");
}

static int
_luoClientReadSingleLineReply(int fd) {
    luo_str reply = _luoClientReadLine(fd);

    if (reply == NULL) {
        return LUO_CLIENT_ERR;
    }

    printf("%s\n", reply);

    return LUO_CLIENT_OK;
}

static int
_luoClientReadBulkReply(int fd) {
    char *reply;
    char crlf[2];
    int  bulk_len;

    luo_str reply_len = _luoClientReadLine(fd);

    if (reply_len == NULL) {
        return LUO_CLIENT_ERR;
    }

    bulk_len = atoi(reply_len);

    if (bulk_len == -1) {
        luoStrFree(reply_len);
        luoLog(LUO_LOG_ERROR, "Is nil.");
        return LUO_CLIENT_ERR;
    }

    reply = luoMalloc((size_t) bulk_len);

    luoTcpRead(fd, reply, bulk_len);
    luoTcpRead(fd, crlf, 2);

    if (bulk_len && fwrite(reply, (size_t) bulk_len, 1, stdout) == 0) {
        luoFree(reply);
        return LUO_CLIENT_ERR;
    }

    if (isatty(fileno(stdout)) && reply[bulk_len - 1] != '\n') {
        printf("\n");
    }

    luoFree(reply);

    return LUO_CLIENT_OK;
}

static int
_luoClientReadMultiBulkReply(int fd) {
    int elements;
    int i = 1;

    luo_str reply_len = _luoClientReadLine(fd);

    if (reply_len == NULL) {
        return LUO_CLIENT_ERR;
    }

    elements = atoi(reply_len);

    if (elements == -1) {
        luoStrFree(reply_len);
        luoLog(LUO_LOG_ERROR, "Is nil.");
        return LUO_CLIENT_ERR;
    }

    if (elements == 0) {
        luoLog(LUO_LOG_NOTICE, "Empty list or set.");
    }

    while (elements--) {
        printf("%d. ", i);
        if (_luoClientReadLine(fd)) {
            return LUO_CLIENT_ERR;
        }
        i++;
    }

    return LUO_CLIENT_OK;
}

static int
_luoClientReadReply(int fd) {
    char type;

    if (luoTcpRead(fd, &type, 1) <= 0) {
        exit(1);
    }

    luoLog(LUO_LOG_DEUBG, "type = %c", type);

    switch (type) {
        case '-':
            luoLog(LUO_LOG_ERROR, "Error: %s", _luoClientReadSingleLineReply(fd));
            return LUO_CLIENT_ERR;
        case '+':
        case ':':
            return _luoClientReadSingleLineReply(fd);
        case '$':
            return _luoClientReadBulkReply(fd);
        case '*':
            return _luoClientReadMultiBulkReply(fd);
        default:
            luoLog(LUO_LOG_ERROR, "Pootocol error, got '%c' as reply type byte.", type);
            return LUO_CLIENT_ERR;
    }
}

static int
_luoClinetSendCommand(int argc, char **argv) {
    int     fd;
    int     i;
    int     result_val = 0;
    luo_str cmd        = luoStrEmpty();

    struct luo_command_s *luo_command;

    luo_command = _luoClientLookupCommand(argv[0]);

    if (!luo_command) {
        luoLog(LUO_LOG_ERROR, "Unknow command '%s'", argv[0]);
        return LUO_CLIENT_ERR;
    }

    if ((luo_command->arity > 0 && argc != luo_command->arity) ||
        (luo_command->arity < 0 && argc < -luo_command->arity)) {
        luoLog(LUO_LOG_ERROR, "Wrong number of arguments for '%s'", luo_command->name);
        return LUO_CLIENT_ERR;
    }

    fd = _luoClientConnect();

    if (fd == -1) {
        return LUO_CLIENT_ERR;
    }

    for (i = 0; i < argc; ++i) {
        if (i != 0) {
            cmd = luoStrCat(cmd, " ");
        }

        if (i == argc - 1 && luo_command->flags & LUO_CMD_BULK) {
            cmd = luoStrCatPrintf(cmd, "%d", luoStrLen(argv[i]));
        } else {
            cmd = luoStrCatLen(cmd, argv[i], luoStrLen(argv[i]));
        }
    }

    cmd = luoStrCat(cmd, "\r\n");

    if (luo_command->flags & LUO_CMD_BULK) {
        cmd = luoStrCatLen(cmd, argv[argc - 1], luoStrLen(argv[argc - 1]));
        cmd = luoStrCat(cmd, "\r\n");
    }

    luoLog(LUO_LOG_DEUBG, "cmd = %s", cmd);

    luoTcpWrite(fd, cmd, luoStrLen(cmd));

    result_val = _luoClientReadReply(fd);

    close(fd);

    if (result_val) {
        return result_val;
    }

    return LUO_CLIENT_OK;
}

int
main(int argc, char **argv) {
    int  first_arg;
    int  i;
    char **argv_copy;

    struct luo_command_s *luo_command;

    // 初始化客户端配置
    luo_client_config.host_ip   = luoStrNew(LUO_SERVER_TCP_IP);
    luo_client_config.host_port = LUO_SERVER_TCP_PORT;

    first_arg = _luoClientParseOptions(argc, argv);
    argc -= first_arg;
    argv += first_arg;

    argv_copy = luoMalloc(sizeof(char *) * argc + 1);

    for (i = 0; i < argc; ++i) {
        argv_copy[i] = luoStrNew(argv[i]);
    }

    if (argc < 1) {
        luoLog(LUO_LOG_ERROR, "Usage: luodb-client [-h host] [-p port] cmd arg1 arg2 arg3 ... argN");
        exit(1);
    }

    luo_command = _luoClientLookupCommand(argv[0]);

    if (luo_command != NULL) {
        if (luo_command->arity > 0 && argc == luo_command->arity - 1) {
            luo_str last_arg = _luoClientReadArgFromStdin();
            argv_copy[argc] = last_arg;
            argc++;
        }
    }

    return _luoClinetSendCommand(argc, argv_copy);
}