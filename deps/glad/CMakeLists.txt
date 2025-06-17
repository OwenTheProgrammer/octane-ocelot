
project(glad LANGUAGES C)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(TARGET_DIR ${PROJECT_SOURCE_DIR}/debug)
else()
    set(TARGET_DIR ${PROJECT_SOURCE_DIR}/release)
endif()


add_library(${PROJECT_NAME} STATIC
    ${TARGET_DIR}/src/glad.c
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        "${TARGET_DIR}/include"
)

