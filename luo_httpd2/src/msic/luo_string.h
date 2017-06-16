//
// Created by luoliang on 16/4/12.
//

#ifndef LUO_HTTPD_LUO_STRING_H
#define LUO_HTTPD_LUO_STRING_H

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef __cplusplus
extern "C" {
#endif

// 字符串转换为小写
#define luo_tolower(c)  (char) ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)
// 字符串转换为大写
#define luo_toupper(c)  (char) ((c >= 'a' && c <= 'z') ? (c & ~0x20) : c)
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

#ifdef __cplusplus
}
#endif

#endif //LUO_HTTPD_LUO_STRING_H
