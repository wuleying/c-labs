//
// Created by luoliang on 16/7/8.
//

#ifndef PROJECT_LUO_STRING_H
#define PROJECT_LUO_STRING_H

#include "../core/luo_core.h"

// 判断字符x是否为空白符
#define luo_isspace(x)  isspace((int) (x))

/**
 * 拷贝字符串
 */
char *luo_string_dup(const char *str);

/**
 * 删除字符串头尾空格
 */
unsigned luo_string_strip(char *str);

/**
 * 字符串转小写
 */
const char *luo_string_lower(const char *in, char *out, unsigned len);

#endif //PROJECT_LUO_STRING_H
