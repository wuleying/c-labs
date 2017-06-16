//
// Created by luoliang on 16/5/31.
//

#include "luo_config.h"
#include "luo_core.h"
#include "luo.h"

int luo_cdecl
main(int argc, char *argv[]) {
    luo_pool_t *pool = luo_pool_create(16);

    luo_reset_pool(pool);

    luo_destroy_pool(pool);

    return LUO_OK;
}