cmake_minimum_required(VERSION 3.23)

project(modelio LANGUAGES C)

add_library(${PROJECT_NAME} STATIC
    src/internal.h
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        console
)

add_dependencies(${PROJECT_NAME}
    console
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        "${PROJECT_SOURCE_DIR}/include"

    PRIVATE
        "${PROJECT_SOURCE_DIR}/src"
)

add_subdirectory(src/obj)
