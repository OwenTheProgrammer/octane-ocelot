cmake_minimum_required(VERSION 3.21)

project(dbuf LANGUAGES C)

add_library(${PROJECT_NAME} STATIC
    src/bswap.c src/endian.c
    src/dbuf.c  src/dbuf_rw.c
)


target_include_directories(${PROJECT_NAME}
    PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
)

