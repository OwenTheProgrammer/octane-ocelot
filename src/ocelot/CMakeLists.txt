
add_library(ocelot OBJECT
	bswap.c endian.c
	dbuf.c dbuf_rw.c
    print_utils.c
)

target_link_libraries(ocelot PUBLIC
    cglm
)

target_include_directories(ocelot
    PUBLIC "${CMAKE_SOURCE_DIR}/include"
    PUBLIC "${CMAKE_SOURCE_DIR}/libs/cglm/include"
)
