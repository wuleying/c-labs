//
// Created by luoliang1 on 2017/7/11.
//

#ifndef LUODB_CONSTANT_H
#define LUODB_CONSTANT_H

// 系统版本
#define LUODB_VERSION "0.0.1"

// 函数返回状态
#define LUO_RETURN_OK       0
#define LUO_RETURN_ERR      1

// 命令类型
#define LUO_CMD_INLINE      1
#define LUO_CMD_BULK        2

// 默认IP
#define LUO_SERVER_TCP_IP   "127.0.0.1"
// 默认TCP端口
#define LUO_SERVER_TCP_PORT 20088

#endif //LUODB_CONSTANT_H