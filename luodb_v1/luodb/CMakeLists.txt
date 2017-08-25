# cmake最低版本
cmake_minimum_required(VERSION 2.8.7)
# 指定头文件的搜索路径
include_directories(${PROJECT_SOURCE_DIR})
# 设置可执行文件输出目路
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# 设置编译选项
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -w")
# 编译参数 server|client
option("BUILD_MODE" "Build mode" "server")

if (${BUILD_MODE} STREQUAL "client")

    # 客户端源文件
    set(LUODB_SOURCE_SRC
            config/config.c
            client/client.c
            memory/mem.c
            net/tcp.c
            structure/dlist.c
            structure/str.c
            util/log.c
            util/file.c
            )

else ()

    # 服务端源文件
    set(LUODB_SOURCE_SRC
            config/config.c
            db/db.c
            server/server.c
            event/event.c
            event/event_epoll.c
            event/event_kqueue.c
            event/event_select.c
            memory/mem.c
            net/tcp.c
            object/object.c
            structure/dict.c
            structure/dlist.c
            structure/str.c
            thirdparty/lzf/lzf_c.c
            thirdparty/lzf/lzf_d.c
            util/log.c
            util/file.c
            )

endif ()

add_executable(${PROJECT_NAME}-${BUILD_MODE} ${LUODB_SOURCE_SRC})