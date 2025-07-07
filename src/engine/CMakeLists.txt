
project(engine LANGUAGES C)

add_library(${PROJECT_NAME} STATIC
    internal.h init.c
)

add_subdirectory(runtime)
add_subdirectory(graphics)
add_subdirectory(scene)


target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        GLFW_INCLUDE_NONE
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        octane ocelot filepath modelio glfw glad cglm
)

add_dependencies(${PROJECT_NAME}
    octane ocelot filepath modelio glfw glad cglm
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        "${CMAKE_SOURCE_DIR}/include"
    PRIVATE
        "${CMAKE_SOURCE_DIR}/src"
)

