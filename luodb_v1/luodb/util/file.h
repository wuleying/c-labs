//
// Created by luoliang1 on 2017/5/8.
//

#ifndef LUODB_UTIL_FILE_H
#define LUODB_UTIL_FILE_H

#include <sys/stat.h>
#include <unistd.h>
#include <memory.h>

#include <luodb/structure/str.h>

void luoFileMakeDirs(char *mul_dir);

void luoFileSavePid(char *pid_file_path, pid_t pid);

#endif //LUODB_UTIL_FILE_H
