
# Compile the octane source code into object files
add_library(octane OBJECT
    oct_internal.h
    oct.c oct_decode.c oct_encode.c oct_serialize.c oct_tableSearch.c
    ibuf.c vbuf.c
)

target_include_directories(octane PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)
