//
// Created by luoliang on 16/4/12.
//

#ifndef HTTPD_CORE_LUO_CONFIG_H
#define HTTPD_CORE_LUO_CONFIG_H

#include <stdio.h>
#include <string.h>

#include "../msic/luo_ini.h"

// 配置文件地址
#define CONFIG_FILE_PATH "/usr/local/etc/luo_httpd/httpd.ini"

// 配置结构体
typedef struct luo_config_s {
    const char *addr;   // 监听地址
    const char *port;   // 监听端口号
} luo_config_t;

// 初始化配置
int luo_config_init(luo_config_t *config);

#endif // HTTPD_CORE_LUO_CONFIG_H