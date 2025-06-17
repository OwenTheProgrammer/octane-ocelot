cmake_minimum_required(VERSION 3.21)

project(dbuf LANGUAGES C)

add_library(${PROJECT_NAME} STATIC
	src/dbuf.c
	src/dbuf_rw.c
)

add_dependencies(${PROJECT_NAME}
	byteconv
)

target_include_directories(${PROJECT_NAME}
	PUBLIC
		"${PROJECT_SOURCE_DIR}/include"

	PRIVATE
		"${PROJECT_SOURCE_DIR}/include/data"
)

target_link_libraries(${PROJECT_NAME}
	PUBLIC
		byteconv
)
