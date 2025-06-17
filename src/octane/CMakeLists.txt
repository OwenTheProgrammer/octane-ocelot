
project(octane LANGUAGES C)

add_library(${PROJECT_NAME} STATIC
    oct.c oct_decode.c oct_encode.c
    oct_loadBindings.c
    oct_parseAtom.c oct_parseEnum.c oct_parseScene.c
    ibuf.c vbuf.c
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        byteconv dbuf
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        "${CMAKE_SOURCE_DIR}/include"
)