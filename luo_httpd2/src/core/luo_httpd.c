//
// Created by luoliang on 16/4/12.
//

#include "luo_httpd.h"

int main(int argc, char *argv[]) {
    luo_config_t config = {0};

    if (-1 == luo_config_init(&config)) {
        return -1;
    }

    int sock_type = 0;

    int listen_fd = luo_handle_listen(config.addr, config.port, &sock_type);

    if (listen_fd < 0) {
        fprintf(stderr, "Error: Listen fd is error.\n");
        return -1;
    }

    if(listen(listen_fd, 5) < 0) {
        fprintf(stderr, "Error: Listen is error.\n");
        return -1;
    }

    printf("luo_httpd running on port %s\n", config.port);

    // 事件循环
    if (-1 == luo_handle_loop(listen_fd)) {
        fprintf(stderr, "Error: Handle loop is error.\n");
        return -1;
    }

    return 0;
}