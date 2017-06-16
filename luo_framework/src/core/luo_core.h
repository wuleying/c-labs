//
// Created by luoliang on 16/5/31.
//

#ifndef LUO_FRAMEWORK_LUO_CORE_H
#define LUO_FRAMEWORK_LUO_CORE_H

#include "luo_config.h"

typedef struct luo_pool_s  luo_pool_t;
typedef struct luo_chain_s luo_chain_t;

// 包含系统头文件
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ntsid.h>
#include <ctype.h>
#include <sys/types.h>

// 全局常量
#define  LUO_OK          0
#define  LUO_ERROR      -1
#define  LUO_AGAIN      -2
#define  LUO_BUSY       -3
#define  LUO_DONE       -4
#define  LUO_DECLINED   -5
#define  LUO_ABORT      -6

#define LF     (u_char) '\n'
#define CR     (u_char) '\r'
#define CRLF   "\r\n"

// 自定义数据类型
typedef unsigned  luo_unsigned_t;
typedef char      luo_char_t;
typedef u_char    luo_uchar_t;
typedef intptr_t  luo_int_t;
typedef uintptr_t luo_uint_t;
typedef intptr_t  luo_flag_t;

// 包含自定义头文件
#include "luo_string.h"
#include "luo_array.h"
#include "luo_buf.h"
#include "luo_conf_file.h"
#include "luo_file.h"
#include "luo_hash.h"
#include "luo_hashtable.h"
#include "luo_list.h"
#include "luo_log.h"
#include "luo_md5.h"
#include "luo_palloc.h"
#include "luo_queue.h"
#include "luo_times.h"

#endif //LUO_FRAMEWORK_LUO_CORE_H