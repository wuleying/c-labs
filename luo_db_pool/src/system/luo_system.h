//
// Created by luoliang on 16/7/8.
//

#ifndef PROJECT_LUO_SYSTEM_H
#define PROJECT_LUO_SYSTEM_H

#include "../core/luo_core.h"

// 错误号
#define luo_errno() (errno == 0 ? "None" : strerror(errno))

// 调试
#ifdef NDEBUG
#define luo_debug(info, ...)
#else
#define luo_debug(debug, ...) fprintf(stdout, "[DEBUG](%s:%d) " debug "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif

// 错误
#define luo_error(err, ...) {fprintf(stderr, "[ERROR](%s:%d: errno: %s) " err "\n", __FILE__, __LINE__, luo_errno(), ##__VA_ARGS__); exit(1); }

// 警告
#define luo_warn(warn, ...) fprintf(stdout, "[WARN](%s:%d) " warn "\n", __FILE__, __LINE__, ##__VA_ARGS__)

// 信息
#define luo_info(info, ...) fprintf(stdout, "[INFO](%s:%d) " info "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#endif //PROJECT_LUO_SYSTEM_H