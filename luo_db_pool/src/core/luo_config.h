//
// Created by luoliang on 16/7/8.
//

#ifndef PROJECT_LUO_CONFIG_H
#define PROJECT_LUO_CONFIG_H

#include "luo_core.h"

// 配置文件地址
#define CONFIG_FILE_PATH "/usr/local/etc/luo_db_pool/config.ini"

// 配置结构体
typedef struct luo_config_s {
    const char *addr;   // 监听地址
    const char *port;   // 监听端口号
} luo_config_t;

// 初始化配置
int luo_config_init(luo_config_t *config);

#endif //PROJECT_LUO_CONFIG_H
