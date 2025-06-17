cmake_minimum_required(VERSION 3.21)

project(hashtable LANGUAGES C)

add_library(${PROJECT_NAME} STATIC
    src/hash_table.c
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        "${PROJECT_SOURCE_DIR}/include"

    PRIVATE
        "${PROJECT_SOURCE_DIR}/include/data"
)