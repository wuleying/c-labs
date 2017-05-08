//
// Created by luoliang1 on 2017/5/8.
//

#include <luodb/util/file.h>

void
luoMakeDirs(char *mul_dir) {
    int  i;
    long len;

    luo_str str = luoStrNew(mul_dir);
    len = luoStrLen(str);

    // 检查目录是否存在
    if (access(str, 0) == 0) {
        return;
    }

    for (i = 0; i < len; i++) {
        if (str[i] == '/') {
            str[i] = '\0';
            if (access(str, 0) != 0) {
                mkdir(str, 0755);
            }
            str[i] = '/';
        }
    }
    if (len > 0 && access(str, 0) != 0) {
        mkdir(str, 0755);
    }
}