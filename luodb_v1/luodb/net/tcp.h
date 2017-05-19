//
// _                 _ _             _
// | |_   _  ___   __| | |__   __   _/ |
// | | | | |/ _ \ / _` | '_ \  \ \ / / |
// | | |_| | (_) | (_| | |_) |  \ V /| |
// |_|\__,_|\___/ \__,_|_.__/    \_/ |_|
//
// Created by luoliang1 on 2017/5/4.
//

#ifndef LUODB_TCP_H
#define LUODB_TCP_H

#include <stdarg.h>
#include <stdio.h>
#include <errno.h>
#include <memory.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>

#define LUO_TCP_OK                  0
#define LUO_TCP_ERR                 -1

#define LUO_TCP_ERR_LEN             256

#define LUO_TCP_CONNECT_NONE        0
#define LUO_TCP_CONNECT_NONBLOCK    1

int luoTcpConnect(char *err, char *addr, int port);

int luoTcpNonBlockConnect(char *err, char *addr, int port);

int luoTcpRead(int fd, char *buf, int count);

int luoTcpWrite(int fd, char *buf, int count);

int luoTcpResolve(char *err, char *host, char *ip_buf);

int luoTcpServer(char *err, int port, char *bind_addr);

int luoTcpAccept(char *err, int server_sock, char *ip, int *port);

int luoTcpNonBlock(char *err, int fd);

int luoTcpNoDelay(char *err, int fd);

int luoTcpKeepAlive(char *err, int fd);

#endif //LUODB_TCP_H
