CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

SET(SOURCE_SRC
        memory/mem.c
        structure/dict.c
        structure/str.c
        core/luodb.c
        )

ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCE_SRC})