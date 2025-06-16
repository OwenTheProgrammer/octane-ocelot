
project(ocelot LANGUAGES C)


add_library(${PROJECT_NAME} STATIC
    print_utils.c
    color_convert.c texture.c crc.c uuid.c
    file_bmp.c
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        ocelot_utils
)