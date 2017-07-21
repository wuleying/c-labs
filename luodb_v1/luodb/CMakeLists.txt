CMAKE_MINIMUM_REQUIRED(VERSION 2.8.7)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 设置编译选项
#SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -w")

SET(LUODB_SERVER_SOURCE_SRC
        config/config.c
        core/server.c
        event/event.c
        event/event_epoll.c
        event/event_kqueue.c
        event/event_select.c
        memory/mem.c
        net/tcp.c
        structure/dict.c
        structure/dlist.c
        structure/str.c
        thirdparty/lzf/lzf_c.c
        thirdparty/lzf/lzf_d.c
        util/log.c
        util/file.c
        )


SET(LUODB_CLIENT_SOURCE_SRC
        config/config.c
        core/client.c
        memory/mem.c
        net/tcp.c
        structure/dlist.c
        structure/str.c
        util/log.c
        util/file.c
)

ADD_EXECUTABLE(${PROJECT_NAME}-server ${LUODB_SERVER_SOURCE_SRC})
ADD_EXECUTABLE(${PROJECT_NAME}-client ${LUODB_CLIENT_SOURCE_SRC})