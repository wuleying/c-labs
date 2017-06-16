//
// Created by luoliang on 16/7/8.
//

#ifndef LUO_DB_POOL_LUO_CORE_H
#define LUO_DB_POOL_LUO_CORE_H

// 全局常量
#define  LUO_OK          0
#define  LUO_ERROR      -1
#define  LUO_AGAIN      -2
#define  LUO_BUSY       -3
#define  LUO_DONE       -4
#define  LUO_DECLINED   -5
#define  LUO_ABORT      -6

// 换行符常量
#define LF     (u_char) '\n'
#define CR     (u_char) '\r'
#define CRLF   "\r\n"

// 包含系统头文件
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>

// 自定义数据类型
typedef intptr_t  luo_int_t;
typedef uintptr_t luo_uint_t;
typedef intptr_t  luo_flag_t;

// 包含自定义头文件
#include "../system/luo_system.h"
#include "../system/luo_memory.h"
#include "../system/luo_time.h"
#include "../system/luo_thread.h"
#include "../system/luo_string.h"
#include "../system/luo_buffer.h"
#include "../system/luo_dict.h"
#include "../system/luo_config_file.h"

#include "luo_config.h"

#endif //LUO_DB_POOL_LUO_CORE_H