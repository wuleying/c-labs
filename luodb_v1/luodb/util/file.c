//
// Created by luoliang1 on 2017/5/8.
//

#include <luodb/util/file.h>

// 递归生成目录
void
luoFileMakeDirs(char *mul_dir) {
    int  i;
    long len;

    // 目录路径 进行类型转换
    luo_str str = luoStrNew(mul_dir);
    // 获取字符串长度
    len = luoStrLen(str);

    // 检查目录是否存在
    if (access(str, 0) == 0) {
        return;
    }

    // 循环路径,生成多级目录
    for (i = 0; i < len; i++) {
        if (str[i] == '/') {
            str[i] = '\0';
            if (access(str, 0) != 0) {
                mkdir(str, LUO_FILE_RW);
            }
            str[i] = '/';
        }
    }

    // 循环设置目录权限
    if (len > 0 && access(str, 0) != 0) {
        mkdir(str, LUO_FILE_RW);
    }
}

// 文件保存pid
void
luoFileSavePid(char *pid_file_path, pid_t pid) {
    // 文件资源句柄
    FILE *fp = fopen(pid_file_path, "w");
    if (fp) {
        // 写入pid
        fprintf(fp, "%d\n", pid);
        // 关闭文件句柄
        fclose(fp);
    }
}