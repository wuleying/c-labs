//
//  _                 _ _             _
// | |_   _  ___   __| | |__   __   _/ |
// | | | | |/ _ \ / _` | '_ \  \ \ / / |
// | | |_| | (_) | (_| | |_) |  \ V /| |
// |_|\__,_|\___/ \__,_|_.__/    \_/ |_|
//
// Created by luoliang1 on 2017/7/11.
//

#ifndef LUODB_COMMON_CONSTANT_H
#define LUODB_COMMON_CONSTANT_H

// 服务端默认IP
#define LUO_SERVER_TCP_IP           "127.0.0.1"
// 服务端默认TCP端口
#define LUO_SERVER_TCP_PORT         20088

// 全局状态
#define LUO_OK                      0
#define LUO_ERR                     1

// 命令类型
#define LUO_CMD_INLINE              1
#define LUO_CMD_BULK                2

#define LUO_REPL_NONE               0
#define LUO_REPL_CONNECT            1
#define LUO_REPL_CONNECTED          2
#define LUO_REPL_WAIT_BGSAVE_START  3
#define LUO_REPL_WAIT_BGSAVE_END    4
#define LUO_REPL_SEND_BULK          5
#define LUO_REPL_ONLINE             6

// 客户端flags
#define LUO_CLIENT_CLOSE            1
#define LUO_CLIENT_SLAVE            2
#define LUO_CLIENT_MASTER           4
#define LUO_CLIENT_MONITOR          8

#endif //LUODB_COMMON_CONSTANT_H