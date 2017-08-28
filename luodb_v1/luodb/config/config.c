//
//  _                 _ _             _
// | |_   _  ___   __| | |__   __   _/ |
// | | | | |/ _ \ / _` | '_ \  \ \ / / |
// | | |_| | (_) | (_| | |_) |  \ V /| |
// |_|\__,_|\___/ \__,_|_.__/    \_/ |_|
//
// Created by luoliang1 on 2017/5/5.
//

#include <luodb/config/config.h>

// yes/no转换为int型1/0/-1
static int
_yesOrNoToInt(char *str) {
    if (!strcasecmp(str, "yes")) {
        return 1;
    } else if (!strcasecmp(str, "no")) {
        return 0;
    } else {
        return -1;
    }
}

/* 接口实现 */
void
initServerConfig() {
    luo_server.daemonize            = 0;
    luo_server.bind_addr            = luoStrNew(LUO_SERVER_TCP_IP);
    luo_server.port                 = LUO_SERVER_TCP_PORT;
    luo_server.pid_file_path        = NULL;
    luo_server.log_file_dir         = NULL;
    luo_server.log_level            = LUO_LOG_DEBUG;
    luo_server.master               = NULL;
    luo_server.clients              = NULL;
    luo_server.slaves               = NULL;
    luo_server.monitors             = NULL;
    luo_server.repl_state           = 0;
    luo_server.max_clients          = 5;
    luo_server.cron_loops           = 0;
    luo_server.bg_save_inprogress   = 0;
    luo_server.last_save            = NULL;
    luo_server.dirty                = 0;
    luo_server.used_memory          = 0;
    luo_server.stat_start_time      = NULL;
    luo_server.stat_commands_num    = 0;
    luo_server.stat_connections_num = 0;
    luo_server.event_loop           = NULL;
    luo_server.db                   = NULL;
    luo_server.db_num               = LUO_DEFAULT_DB_NUMBER;
}

// 加载服务端配置
void
loadServerConfig(char *file_name) {
    // 配置文件操作句柄
    FILE    *fp      = fopen(file_name, "r");
    // 存放配置项的buffer
    char    buf[LUO_CONFIG_LINE_MAX + 1];
    // 配置项行号
    int     line_num = 0;
    // 错误信息
    char    *err     = NULL;
    // 当前行
    luo_str line     = NULL;

    // 打开配置文件失败
    if (!fp) {
        luoLogWarn("Fatal error, can't open config file [%s].", file_name);
        exit(1);
    }

    // 逐行处理配置项
    while (fgets(buf, LUO_CONFIG_LINE_MAX + 1, fp) != NULL) {
        // 配置项参数
        luo_str *argv;

        // 配置项数量
        int argc, i;

        // 行号加1
        line_num++;

        // 当前行分配buf大小内存
        line = luoStrNew(buf);
        // 当前行去掉空格,制表符与换行符
        line = luoStrTrim(line, " \t\r\n");

        // 当前行为注释或为空行
        if (line[0] == '#' || line[0] == '\0') {
            // 释放内存并开始下一行处理
            luoStrFree(line);
            continue;
        }

        // 按空格拆分配置项,并存入数组
        argv = luoStrSplitLen(line, (int) luoStrLen(line), " ", 1, &argc);
        // 将配置项名称转小写
        luoStrToLower(argv[0]);

        // 守护进程
        if (!strcasecmp(argv[0], "daemonize") && argc == 2) {
            luo_server.daemonize = _yesOrNoToInt(argv[1]);

            if (luo_server.daemonize == -1) {
                err = "Argument must be 'yes' or 'no'";
                goto _displayError;
            }
        }

        // 端口
        if (!strcasecmp(argv[0], "port") && argc == 2) {
            luo_server.port = atoi(argv[1]);

            if (luo_server.port < 1 || luo_server.port > 65535) {
                err = "Invalid port";
                goto _displayError;
            }
        }

        // 数据库数量
        if (!strcasecmp(argv[0], "db_num") && argc == 2) {
            luo_server.db_num = (unsigned int) atoi(argv[1]);

            if (luo_server.db_num < 1) {
                err = "Invalid number of db_num";
                goto _displayError;
            }
        }

        // 客户端最大数量
        if (!strcasecmp(argv[0], "max_clients") && argc == 2) {
            luo_server.max_clients = (unsigned int) atoi(argv[1]);
        }

        // pid文件路径
        if (!strcasecmp(argv[0], "pid_file_path") && argc == 2) {
            luo_server.pid_file_path = luoStrdup(argv[1]);
        }

        // 日志文件路径
        if (!strcasecmp(argv[0], "log_file_dir") && argc == 2) {
            luo_server.log_file_dir = luoStrdup(argv[1]);

            if (!strcasecmp(luo_server.log_file_dir, "stdout")) {
                luoFree(luo_server.log_file_dir);
                luo_server.log_file_dir = NULL;
            }
        }

        // 日志级别
        if (!strcasecmp(argv[0], "log_level") && argc == 2) {
            if (!strcasecmp(argv[1], "trace")) {
                luo_server.log_level = LUO_LOG_TRACE;
            } else if (!strcasecmp(argv[1], "debug")) {
                luo_server.log_level = LUO_LOG_DEBUG;
            } else if (!strcasecmp(argv[1], "info")) {
                luo_server.log_level = LUO_LOG_INFO;
            } else if (!strcasecmp(argv[1], "warn")) {
                luo_server.log_level = LUO_LOG_WARN;
            } else if (!strcasecmp(argv[1], "error")) {
                luo_server.log_level = LUO_LOG_ERROR;
            } else if (!strcasecmp(argv[1], "fatal")) {
                luo_server.log_level = LUO_LOG_FATAL;
            } else {
                err = "Invaild log level. Must be one of trace, debug, info, warn, error and fatal.";
                goto _displayError;
            }
        }

        // 释放内存
        for (i = 0; i < argc; ++i) {
            luoStrFree(argv[i]);
        }

        luoFree(argv);
        luoStrFree(line);
    }

    return;

    // 打印错误信息
    _displayError:
    {
        fprintf(stderr, "\n*** FATAL CONFIG FILE ERROR ***\n");
        fprintf(stderr, "Reading the configuration file, at line %d\n", line_num);
        fprintf(stderr, ">>> '%s'\n", line);
        fprintf(stderr, "%s\n", err);
        exit(1);
    }
}