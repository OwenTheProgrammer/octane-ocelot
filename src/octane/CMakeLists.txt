
project(octane LANGUAGES C)

add_library(${PROJECT_NAME} STATIC
    oct.c oct_decode.c oct_encode.c oct_serialize.c oct_loadBindings.c
    ibuf.c vbuf.c
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        ocelot_utils
)