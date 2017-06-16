//
// Created by luoliang on 16/4/18.
//

#include "luo_handle.h"

/**
 * 监听函数
 */
int
luo_handle_listen(const char *restrict host_name, const char *restrict service, int *restrict sock_type) {
    int listen_fd = 0;

    struct addrinfo hints, *result, *p;

    // 分配内存
    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family   = AF_UNSPEC;
    hints.ai_flags    = AI_PASSIVE;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;

    if (0 != getaddrinfo(host_name, service, &hints, &result)) {
        return -1;
    }

    for (p = result; p != NULL; p = p->ai_next) {
        listen_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

        if (-1 == listen_fd) {
            continue;
        }

        if (-1 == bind(listen_fd, p->ai_addr, p->ai_addrlen)) {
            close(listen_fd);
            continue;
        }

        break;
    }

    if (NULL == p) {
        return -2;
    }

    *sock_type = p->ai_family;

    freeaddrinfo(result);

    return listen_fd;
}


/**
 * 事件循环函数
 */
int
luo_handle_loop(int listen_fd) {
    int             client_sock;
    luo_sockaddr_in client_name;
    socklen_t       client_name_len = sizeof(client_name);
    pthread_t       new_thread;

    while (1) {
        client_sock = (accept(listen_fd, (luo_sockaddr *) &client_name, &client_name_len));

        if (-1 == client_sock) {
            fprintf(stderr, "Error: Accept error.\n");

            close(listen_fd);
            return -1;
        }

        if (0 != pthread_create(&new_thread, NULL, luo_handle_accept_request, (void *) &client_sock)) {
            fprintf(stderr, "Error: Pthread create error.\n");

            close(listen_fd);
            return -1;
        }
    }
}

/**
 * 事件请求函数
 */
void *
luo_handle_accept_request(void *t_client) {
    int client = *(int *) t_client;
    int is_cgi = 0;
    int get_line;

    char buf[BUFSIZ];
    char mothod[BUFSIZ];
    char url[BUFSIZ];
    char paht[BUFSIZ];

    char *query_string = NULL;

    size_t   i = 0;
    size_t   j = 0;
    luo_stat st;

    // 获取一行数据 写入buff
    get_line  = luo_handle_get_line(client, buf, sizeof(buf));

    // 获取请求方式
    while (!luo_isspace(buf[j]) && (sizeof(mothod) - 1 > i)) {
        mothod[i] = buf[j];
        i++;
        j++;
    }
    mothod[i] = '\0';

    // 判断请求方式是否合法
    if (0 == strcasecmp(mothod, "GET") && 0 == strcasecmp(mothod, "POST")) {
        luo_handle_unimplemented(client);

        close(client);
        return NULL;
    }

    return NULL;
}

/**
 * 获取一行数据 并写入buff
 */
int
luo_handle_get_line(int sock, char *buf, int buf_size) {
    int     i = 0;
    ssize_t n;
    char    c = '\0';

    while ((i < buf_size - 1) && c != '\n') {
        n = recv(sock, &c, 1, 0);

        if (n > 0) {
            if (c == '\r') {
                n = recv(sock, &c, 1, MSG_PEEK);

                if ((n > 0) && (c == '\n')) {
                    recv(sock, &c, 1, 0);
                }
                else {
                    c = '\n';
                }
            }

            buf[i] = c;
            i++;
        }
        else {
            c = '\n';
        }
    }

    buf[i] = '\0';
    return i;
}

/**
 * 输出http异常
 */
void
luo_handle_exception(int client, int code, const char *title, const char *content) {
    char buf[BUFSIZ];

    sprintf(buf, "HTTP/1.0 %d %s\r\n", code, title);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, SERVER_DESC);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "Content-Type: text/html\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "<HTML><HEAD><TITLE>%d %s\r\n", code, title);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "</TITLE></HEAD>\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "<BODY><P>%s</P>\r\n", content);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "</BODY></HTML>\r\n");
    send(client, buf, strlen(buf), 0);
}

/**
 * 不支持的请求方式
 */
void
luo_handle_unimplemented(int client) {
    luo_handle_exception(client, 501, "Not Implemented", "501 Not Implemented.");
}
