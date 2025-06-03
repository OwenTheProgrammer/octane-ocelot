
add_library(ocelot OBJECT
	bswap.c endian.c
	dbuf.c dbuf_rw.c
	math_impl.c
    print_utils.c
)

target_link_libraries(ocelot PUBLIC
    m
)

target_include_directories(ocelot
    PUBLIC "${CMAKE_SOURCE_DIR}/include"
)
