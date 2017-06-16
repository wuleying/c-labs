//
// Created by luoliang on 16/7/8.
//

#include "luo_core.h"

int main(int argc, char *argv[]) {
    luo_config_t config = {0};

    if (LUO_ERROR == luo_config_init(&config)) {
        return LUO_ERROR;
    }

    luo_debug("config.addr:%s", config.addr);
    luo_debug("config.port:%s", config.port);

    return LUO_OK;
}