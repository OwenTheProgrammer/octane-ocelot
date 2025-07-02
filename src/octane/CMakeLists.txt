
project(octane LANGUAGES C)

add_library(${PROJECT_NAME} STATIC
    print_utils.c
)

add_subdirectory(oct)
add_subdirectory(ibuf)
add_subdirectory(vbuf)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        dbuf filepath platform
)

add_dependencies(${PROJECT_NAME}
    dbuf filepath platform
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        "${CMAKE_SOURCE_DIR}/include"
)
