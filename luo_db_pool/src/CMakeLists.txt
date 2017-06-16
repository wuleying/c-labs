CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

SET(SOURCE_SRC
        core/luo_core.h
        core/luo_config.c
        core/luo.c
        db/mysql/connection.c
        exception/luo_assert.c
        exception/luo_exception.c
        system/luo_buffer.c
        system/luo_memory.c
        system/luo_string.c
        system/luo_system.c
        system/luo_thread.c
        system/luo_time.c
        system/luo_dict.c
        system/luo_config_file.c
)

ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCE_SRC})

INSTALL(TARGETS ${PROJECT_NAME} DESTINATION bin)
INSTALL(FILES ${PROJECT_SOURCE_DIR}/conf/config.ini DESTINATION etc/${PROJECT_NAME} COMPONENT config)