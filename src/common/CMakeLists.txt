
project(ocelot_utils LANGUAGES C)

add_library(${PROJECT_NAME} STATIC
	bswap.c endian.c
	dbuf.c dbuf_rw.c
)

target_include_directories(${PROJECT_NAME}
	PUBLIC
		"${CMAKE_SOURCE_DIR}/include"
)