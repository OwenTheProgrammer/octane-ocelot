
project(engine LANGUAGES C)

add_library(${PROJECT_NAME} STATIC
    camera.c model.c state.c window.c
    scene.c scene_load.c
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        octane ocelot cglm glfw glad
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        GLFW_INCLUDE_NONE
)
