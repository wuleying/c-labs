//
// Created by luoliang on 16/4/14.
//

#ifndef LUO_HTTPD_LUO_INI_H
#define LUO_HTTPD_LUO_INI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "luo_dict.h"
#include "luo_string.h"
#include "luo_error.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ASCII_LINE_SIZE     BUFSIZ
#define INT_BUF_SIZE        (ASCII_LINE_SIZE + 1)
#define TMP_BUF_SIZE        ((ASCII_LINE_SIZE * 2) + 1)

#define INI_INVALID_KEY     ((char *) - 1)

// 文件行状态码
typedef enum _luo_line_status_s {
    LINE_UNPROCESSED,
    LINE_ERROR,
    LINE_EMPTY,
    LINE_COMMENT,
    LINE_SECTION,
    LINE_VALUE
} luo_line_status_t;

/**
 * 加载ini文件 并存为字典对象
 */
luo_dict_t *luo_ini_load(const char *ini_file_name);

/**
 * 按行处理 并返回行状态码
 */
static luo_line_status_t luo_ini_line(const char *input_line, char *section, char *key, char *val);

/**
 * 返回字典对象的section:key对应的字串值
 */
const char *luo_ini_get_string(luo_dict_t *dict, const char *key);

/**
 * 返回字典对象的section:key对应的整型值
 */
int luo_ini_get_int(luo_dict_t *dict, const char *key);

/**
 * 返回字典对象的section:key对应的双精度浮点值
 */
double luo_ini_get_double(luo_dict_t *dict, const char *key);

/**
 * 释放
 */
void luo_ini_free(luo_dict_t *dict);

#ifdef __cplusplus
}
#endif

#endif //LUO_HTTPD_LUO_INIPARSER_H
