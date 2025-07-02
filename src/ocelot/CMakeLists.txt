
project(ocelot LANGUAGES C)


add_library(${PROJECT_NAME} STATIC
    print_utils.c
    color_convert.c texture.c crc.c uuid.c
    file_bmp.c
)

add_subdirectory(scene)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        dbuf octane filepath platform
)

add_dependencies(${PROJECT_NAME}
    dbuf filepath platform
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        "${CMAKE_SOURCE_DIR}/include"
)
