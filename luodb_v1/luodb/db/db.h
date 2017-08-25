//
// Created by luoliang1 on 2017/8/25.
//

#ifndef LUODB_DB_DB_H
#define LUODB_DB_DB_H

#include <luodb/structure/dict.h>

// db结构体
typedef struct luo_db_s {
    luo_dict *dict;
    luo_dict *expires;
    int      id;
} luo_db;

#endif //LUODB_DB_H
