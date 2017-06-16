//
// Created by luoliang on 16/7/12.
//

#include "luo_core.h"

// 初始化配置
int
luo_config_init(luo_config_t *config) {
    luo_dict_t *ini_dict;

    // 加载ini文件 存为字典对象
    ini_dict = luo_ini_load(CONFIG_FILE_PATH);

    if (NULL == ini_dict) {
        luo_error("Cannot parse file.");
    }

    config->addr = luo_ini_get_string(ini_dict, "httpd.addr");
    if (NULL == config->addr) {
        luo_error("Config.addr is empty.");
    }

    config->port = luo_ini_get_string(ini_dict, "httpd.port");
    if (NULL == config->port) {
        luo_error("Config.port is empty.");
    }

    return LUO_OK;
}