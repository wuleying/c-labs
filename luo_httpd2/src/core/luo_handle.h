//
// Created by luoliang on 16/4/18.
//

#ifndef LUO_HTTPD_LUO_HANDLE_H
#define LUO_HTTPD_LUO_HANDLE_H

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <pthread.h>

#include "luo_config.h"

// 结构体声明
typedef struct sockaddr_in luo_sockaddr_in;
typedef struct sockaddr    luo_sockaddr;
typedef struct stat        luo_stat;

// sock默认值
#define SOCK_DEFAULT -1

// 服务描述
#define SERVER_DESC "Server: luo_httpd/0.0.2\r\n"

/**
 * 监听函数
 */
int luo_handle_listen(const char *restrict host_name, const char *restrict service, int *restrict sock_type);

/**
 * 事件循环函数
 */
int luo_handle_loop(int listen_fd);

/**
 * 事件请求函数
 */
void *luo_handle_accept_request(void *t_client);

/**
 * 获取一行数据 并写入buf
 */
int luo_handle_get_line(int sock, char *buf, int buf_size);

/**
 * 输出http异常
 */
void luo_handle_exception(int client, int code, const char *title, const char *content);

/**
 * 不支持的请求方式
 */
void luo_handle_unimplemented(int client);

#endif //LUO_HTTPD_LUO_HANDLE_H