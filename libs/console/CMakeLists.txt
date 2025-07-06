cmake_minimum_required(VERSION 3.21)

project(console LANGUAGES C)

add_library(${PROJECT_NAME} STATIC
    src/log.c
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
)
