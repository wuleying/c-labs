//
//  _                 _ _             _
// | |_   _  ___   __| | |__   __   _/ |
// | | | | |/ _ \ / _` | '_ \  \ \ / / |
// | | |_| | (_) | (_| | |_) |  \ V /| |
// |_|\__,_|\___/ \__,_|_.__/    \_/ |_|
//
// Created by luoliang1 on 2017/5/4.
//

#include <luodb/net/tcp.h>

// 设置TCP错误消息
static void
_luoTcpSetError(char *err, const char *fmt, ...) {
    va_list ap;

    if (!err) {
        return;
    }

    va_start(ap, fmt);
    vsnprintf(err, LUO_TCP_ERR_LEN, fmt, ap);
    va_end(ap);
}

static int
luoTcpGenericConnect(char *err, char *addr, int port, int flags) {
    int s, on = 1;

    struct sockaddr_in sa;

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        _luoTcpSetError(err, "Creating socket: %s\n", strerror(errno));

        return LUO_TCP_ERR;
    }

    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    sa.sin_family = AF_INET;
    sa.sin_port   = htons(port);

    if (inet_aton(addr, &sa.sin_addr) == 0) {
        struct hostent *he;

        he = gethostbyname(addr);

        if (he == NULL) {
            _luoTcpSetError(err, "Can't resolve: %s\n", addr);
            close(s);

            return LUO_TCP_ERR;
        }

        memcpy(&sa.sin_addr, he->h_addr, sizeof(struct in_addr));
    }

    if (flags & LUO_TCP_CONNECT_NONBLOCK) {
        if (luoTcpNonBlock(err, s) != LUO_TCP_OK) {
            return LUO_TCP_ERR;
        }
    }

    if (connect(s, (struct sockaddr *) &sa, sizeof(sa)) == -1) {
        if (errno == EINPROGRESS && (flags & LUO_TCP_CONNECT_NONBLOCK)) {
            return s;
        }

        _luoTcpSetError(err, "Connect: %s\n", strerror(errno));
        close(s);

        return LUO_TCP_ERR;
    }

    return s;
}

// 建立阻塞网络套接字tcp连接
int
luoTcpConnect(char *err, char *addr, int port) {
    return luoTcpGenericConnect(err, addr, port, LUO_TCP_CONNECT_NONE);
}

// 建立非阻塞网络套接字tcp连接
int
luoTcpNonBlockConnect(char *err, char *addr, int port) {
    return luoTcpGenericConnect(err, addr, port, LUO_TCP_CONNECT_NONBLOCK);
}

// TCP通过网络读取文件到buffer中
int
luoTcpRead(int fd, char *buf, int count) {
    ssize_t tcp_read;
    int     total_len = 0;

    while (total_len != count) {
        tcp_read = read(fd, buf, (size_t) (count - total_len));

        if (tcp_read == 0) {
            return total_len;
        }

        if (tcp_read == -1) {
            return LUO_TCP_ERR;
        }

        total_len += tcp_read;
        buf += tcp_read;
    }

    return total_len;
}

// TCP通过网络从buffer中写入文件操作
int
luoTcpWrite(int fd, char *buf, int count) {
    ssize_t tcp_write;
    int     total_len = 0;

    while (total_len != count) {
        tcp_write = write(fd, buf, (size_t) (count - total_len));

        if (tcp_write == 0) {
            return total_len;
        }

        if (tcp_write == -1) {
            return LUO_TCP_ERR;
        }

        total_len += tcp_write;
        buf += tcp_write;
    }

    return total_len;
}

int
luoTcpResolve(char *err, char *host, char *ip_buf) {
    struct sockaddr_in sa;

    sa.sin_family = AF_INET;

    if (inet_aton(host, &sa.sin_addr) == 0) {
        struct hostent *he;

        he = gethostbyname(host);

        if (he == NULL) {
            _luoTcpSetError(err, "Can't resolve: %s\n", host);
        }

        memcpy(&sa.sin_addr, he->h_addr, sizeof(struct in_addr));
    }
    strcpy(ip_buf, inet_ntoa(sa.sin_addr));

    return LUO_TCP_OK;
}

int
luoTcpServer(char *err, int port, char *bind_addr) {
    int s, on = 1;

    struct sockaddr_in sa;

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        _luoTcpSetError(err, "Socket: %s\n", strerror(errno));
        return LUO_TCP_ERR;
    }

    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1) {
        _luoTcpSetError(err, "setsockopt(SO_REUSEADDR): %s\n", strerror(errno));
        close(s);
        return LUO_TCP_ERR;
    }

    memset(&sa, 0, sizeof(sa));

    sa.sin_family      = AF_INET;
    sa.sin_port        = htons(port);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind_addr && inet_aton(bind_addr, &sa.sin_addr) == 0) {
        _luoTcpSetError(err, "Invaild bind address\n");
        close(s);
        return LUO_TCP_ERR;
    }

    if (bind(s, (struct sockaddr *) &sa, sizeof(sa)) == -1) {
        _luoTcpSetError(err, "Bind: %s\n", strerror(errno));
        close(s);
        return LUO_TCP_ERR;
    }

    if (listen(s, 64) == -1) {
        _luoTcpSetError(err, "Listen: %s\n", strerror(errno));
        close(s);
        return LUO_TCP_ERR;
    }

    return s;
}

int
luoTcpAccept(char *err, int server_sock, char *ip, int *port) {
    int                fd;
    struct sockaddr_in sa;
    unsigned int       sa_len;

    while (1) {
        sa_len = sizeof(sa);

        fd = accept(server_sock, (struct sockaddr *) &sa, &sa_len);

        if (fd == -1) {
            if (errno == EINTR) {
                continue;
            } else {
                _luoTcpSetError(err, "Accept: %s\n", strerror(errno));
                return LUO_TCP_ERR;
            }
        }

        break;
    }

    if (ip) {
        strcpy(ip, inet_ntoa(sa.sin_addr));
    }

    if (port) {
        *port = ntohs(sa.sin_port);
    }

    return fd;
}

// 设置非阻塞
int
luoTcpNonBlock(char *err, int fd) {
    int flag;

    if ((flag = fcntl(fd, F_GETFL)) == -1) {
        _luoTcpSetError(err, "fcntl(F_GETFL) %s\n", strerror(errno));
        return LUO_TCP_ERR;
    }

    if (fcntl(fd, F_SETFL, flag | O_NONBLOCK) == -1) {
        _luoTcpSetError(err, "fcntl(F_SETFL, O_NONBLOCK) %s\n", strerror(errno));
        return LUO_TCP_ERR;
    }

    return LUO_TCP_OK;
}

// 将tcp连接设为非延迟性的，即屏蔽Nagle算法
int
luoTcpNoDelay(char *err, int fd) {
    int yes = 1;

    if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes)) == -1) {
        _luoTcpSetError(err, "setsockopt TCP_NODELAY: %s\n", strerror(errno));
        return LUO_TCP_ERR;
    }

    return LUO_TCP_OK;
}

// 开启连接检测，避免对方宕机或网络中断时fd一直阻塞
int
luoTcpKeepAlive(char *err, int fd) {
    int yes = 1;

    if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &yes, sizeof(yes)) == -1) {
        _luoTcpSetError(err, "setsockopt SO_KEEPALIVE: %s\n", strerror(errno));
        return LUO_TCP_ERR;
    }

    return LUO_TCP_OK;
}