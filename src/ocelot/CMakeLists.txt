
project(ocelot LANGUAGES C)


add_library(${PROJECT_NAME} STATIC
    print_utils.c
    color_convert.c texture.c crc.c uuid.c
    file_bmp.c
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        dbuf platform
)

add_dependencies(${PROJECT_NAME}
    dbuf platform
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        "${CMAKE_SOURCE_DIR}/include"
)
