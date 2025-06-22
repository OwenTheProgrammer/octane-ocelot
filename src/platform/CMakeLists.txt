
project(platform LANGUAGES C)

add_library(${PROJECT_NAME} STATIC
    platform.c
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        dbuf
)

add_dependencies(${PROJECT_NAME}
    dbuf
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        "${CMAKE_SOURCE_DIR}/include"
        "${CMAKE_SOURCE_DIR}/libs/dbuf/include" # Hack to shut clangd up
)
