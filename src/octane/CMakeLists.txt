
project(octane LANGUAGES C)

add_library(${PROJECT_NAME} STATIC
    vbuf.c print_utils.c
)

add_subdirectory(oct)
add_subdirectory(ibuf)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        dbuf
)

add_dependencies(${PROJECT_NAME}
    dbuf
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        "${CMAKE_SOURCE_DIR}/include"
)
