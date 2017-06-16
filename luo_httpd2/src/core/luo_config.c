//
// Created by luoliang on 16/4/12.
//

#include "luo_config.h"

// 初始化配置
int
luo_config_init(luo_config_t *config) {
    luo_dict_t *ini_dict;

    // 加载ini文件 存为字典对象
    ini_dict = luo_ini_load(CONFIG_FILE_PATH);

    if (NULL == ini_dict) {
        fprintf(stderr, "Error: Cannot parse file.\n");
        return -1;
    }

    config->addr = luo_ini_get_string(ini_dict, "httpd.addr");
    if (NULL == config->addr) {
        fprintf(stderr, "Error: Config.addr is empty.\n");
        return -1;
    }

    config->port = luo_ini_get_string(ini_dict, "httpd.port");
    if (NULL == config->port) {
        fprintf(stderr, "Error: Config.port is empty.\n");
        return -1;
    }

    return 0;
}
