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

inline static void
luoTcpSetError(char *err, const char *fmt, ...) {
    va_list ap;

    if (!err) {
        return;
    }

    va_start(ap, fmt);
    vsnprintf(err, LUO_TCP_ERR_LEN, fmt, ap);
    va_end(ap);
}

int
luoTcpConnect(char *err, char *addr, int port) {
    return LUO_TCP_OK;
}

int
luoTcpNonBlockConnect(char *err, char *addr, int port) {
    return 0;
}

int
luoTcpRead(int fd, char *buf, int count) {
    return 0;
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
luoTcpWrite(int fd, char *buf, int count) {
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