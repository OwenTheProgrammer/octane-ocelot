
project(filepath LANGUAGES C)

add_library(${PROJECT_NAME} STATIC
    src/filepath.c src/filepath.linux.c
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
)
