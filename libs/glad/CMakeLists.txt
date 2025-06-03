

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(TARGET_DIR ${CMAKE_CURRENT_SOURCE_DIR}/debug)
else()
    set(TARGET_DIR ${CMAKE_CURRENT_SOURCE_DIR}/release)
endif()


add_library(glad STATIC
    ${TARGET_DIR}/src/glad.c
)

target_include_directories(glad PUBLIC
    ${TARGET_DIR}/include
)

