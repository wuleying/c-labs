//
// _                 _ _             _
// | |_   _  ___   __| | |__   __   _/ |
// | | | | |/ _ \ / _` | '_ \  \ \ / / |
// | | |_| | (_) | (_| | |_) |  \ V /| |
// |_|\__,_|\___/ \__,_|_.__/    \_/ |_|
//
// Created by luoliang1 on 2017/5/4.
//

#include <luodb/net/tcp.h>

static void
luoTcpSetError(char *err, const char *fmt, ...) {
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
        luoTcpSetError(err, "Creating socket: %s\n", strerror(errno));

        return LUO_TCP_ERR;
    }

    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    sa.sin_family = AF_INET;
    sa.sin_port   = htons(port);

    if (inet_aton(addr, &sa.sin_addr) == 0) {
        struct hostent *he;

        he = gethostbyname(addr);

        if (he == NULL) {
            luoTcpSetError(err, "Can't resolve: %s\n", addr);
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

        luoTcpSetError(err, "Connect: %s\n", strerror(errno));
        close(s);

        return LUO_TCP_ERR;
    }

    return s;
}

int
luoTcpConnect(char *err, char *addr, int port) {
    return luoTcpGenericConnect(err, addr, port, LUO_TCP_CONNECT_NONE);
}

int
luoTcpNonBlockConnect(char *err, char *addr, int port) {
    return luoTcpGenericConnect(err, addr, port, LUO_TCP_CONNECT_NONBLOCK);
}

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
    return 0;
}

int
luoTcpServer(char *err, int port, char *bind_addr) {
    return 0;
}

int
luoTcpAccept(char *err, int server_sock, char *ip, int *port) {
    return 0;
}

int
luoTcpNonBlock(char *err, int fd) {
    return 0;
}

int
luoTcpNoDelay(char *err, int fd) {
    return 0;
}

int
luoTcpKeepAlive(char *err, int fd) {
    return 0;
}