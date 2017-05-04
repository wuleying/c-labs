//
// Created by luoliang1 on 2017/5/4.
//

#include <stdio.h>

#include <luodb/structure/str.h>

int main(int argc, char *argv[]) {
    luo_str_t val = luo_str_create("hello", 5);

    printf("%s", val);

    return 0;
}