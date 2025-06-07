
add_library(engine OBJECT
    camera.c model.c state.c window.c
    scene.c scene_load.c
)

target_link_libraries(engine PUBLIC
    glfw GL m glad
)

target_compile_definitions(engine PUBLIC
    GLFW_INCLUDE_NONE
)

target_include_directories(engine
    PUBLIC "${CMAKE_SOURCE_DIR}/include"
    PUBLIC "${CMAKE_SOURCE_DIR}/libs/cglm/include"
)
