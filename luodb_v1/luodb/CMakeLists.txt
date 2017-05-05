CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

SET(SOURCE_SRC
        config/config.c
        core/luodb.c
        event/event.c
        event/event_epoll.c
        event/event_kqueue.c
        event/event_select.c
        memory/mem.c
        net/tcp.c
        structure/dict.c
        structure/dlist.c
        structure/str.c
        util/log.c
        )

ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCE_SRC})