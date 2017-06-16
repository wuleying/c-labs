//
// Created by luoliang on 16/5/31.
//

#ifndef LUO_FRAMEWORK_LUO_STRING_H
#define LUO_FRAMEWORK_LUO_STRING_H

#include "luo_config.h"
#include "luo_core.h"

// 字符串结构体
typedef struct luo_str_s {
    // 字符串长度
    size_t len;
    // 字符串值
    u_char *data;
} luo_str_t;


// 设置一个字符串
#define luo_string(str) { sizeof(str) - 1, (u_char *) str }
// 设置一个空字符串
#define luo_null_string { 0, NULL }
// 给一个字符串赋值
#define luo_str_set(str, text) (str)->len = sizeof(text) - 1; (str)->data = (u_char *) text
// 给一个字符串赋空值
#define luo_str_null(str) (str)->len = 0; (str)->data = NULL

// 大写转小写
#define luo_tolower(c) (u_char) ((c >= 'A' && C <= 'Z') ? (c | 0x20) : c)
// 小写转大写
#define luo_toupper(c) (u_char) ((c >= 'a' && C <= 'z') ? (c & ~0x20) : c)
// 判断字符x是否为空白符
#define luo_isspace(x)  isspace((int) (x))

// 比较两个字符串的n个字符
#define luo_strncmp(s1, s2, n) strncmp((const char *) s1, (const char *) s2, n)
// 比较两个字符串
#define luo_strcmp(s1, s2) strcmp((const char *) s1, (const char *) s2)
// 查找s2在s1中第一次出现的位置
#define luo_strstr(s1, s2) strstr((const char *) s1, (const char *) s2)
// 获取字符串长度
#define luo_strlen(s) strlen((const char *) s)
// 查找字符串s中首次出现字符c的位置
#define luo_strchr(s, c) strchr((const char *) s, (int) c)

/* 内存操作 */
// 将buf所指向的内存中的前n个字节的内容全部设置为0
#define luo_memzero(buf, n) (void) memset(buf, 0, n)
// 将buf所指向的内存中的前n个字节的内容全部设置为c
#define luo_memset(buf, c, n) (void) memset(buf, c, n)

// 由src所指内存区域复制n个字节到dst所指内存区域
#define luo_memcpy(dst, src, n) (void) memcpy(dst, src, n)
// 复制内存 返回复制完了dst的最后一个字符的下一个字符的指针
#define luo_cpymem(dst, src, n) (((u_char *) memcpy(dst, src, n)) + (n))
// luo_cpymem 别名
#define luo_copy luo_cpymem

// 从src拷贝n个字符到dst
#define luo_memmove(dst, src, n) (void) memmove(dst, src, n)
// 从src拷贝n个字符到dst 返回复制完了dst的最后一个字符的下一个字符的指针
#define luo_movemem(des, src, n) (((u_char *) memmove(dst, src, n)) + (n))

// 比较内存区域s1和s2的前n个字节
#define luo_memcmp(s1, s2, n) memcmp((const char *) s1, (const char *) s2, n)

#endif //LUO_FRAMEWORK_LUO_STRING_H
