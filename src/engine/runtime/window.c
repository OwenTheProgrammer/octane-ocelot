#include "engine/core/window.h"
#include "cglm/cam.h"
#include "cglm/mat4.h"
#include "cglm/util.h"
#include "engine/core/controls.h"
#include "engine/core/runtime.h"
#include "engine/graphics/camera.h"
#include "engine/graphics/model.h"
#include "engine/graphics/shader.h"
#include <stdio.h>

#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "engine/internal.h"

static void _error_callback(int error, const char* msg)
{
    LOG_API_FATAL("glfw", "Error %i\n\t%s\n", error, msg);
    //fprintf(stderr, "[GLFW]: Error %i\n\t%s\n", error, msg);
}

static void _key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void _window_iconify_callback(GLFWwindow* window, int iconified)
{
    //printf("icon: %i\n", iconified);
}

static void _fb_resize_callback(GLFWwindow* window, int width, int height)
{
    LOG_API_INFO("glfw", "Resized framebuffer (%i x %i)\n", width, height);
    //printf("[glfw]: Resized.\n");
    oce_camera_update_size(&_rt.viewport_camera, (unsigned int)width, (unsigned int)height);
    glViewport(0, 0, width, height);
}

static void _init_camera()
{
    oce_cameraDescriptor params =
    {
        .near_plane = 0.001f,
        .far_plane = 10000.0f,
        .fov_rad = glm_rad(80.0f),

        .width_px = 1280,
        .height_px = 720
    };

    _rt.viewport_camera = oce_camera_init(params);
}

int _oce_init_glfw()
{
    _init_camera();

    glfwSetErrorCallback(_error_callback);

    if(!glfwInit())
        return 0;

    /* OpenGL Version 3.3 */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    /* Start invisible while the engine loads */
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    _rt.gui.window = glfwCreateWindow(1280, 720, "Ocelot Editor", NULL, NULL);

    if(!_rt.gui.window)
    {
        glfwTerminate();
        return 0;
    }

    glfwMakeContextCurrent(_rt.gui.window);

    glfwSetKeyCallback(_rt.gui.window, _key_callback);
    glfwSetFramebufferSizeCallback(_rt.gui.window, _fb_resize_callback);
    glfwSetWindowIconifyCallback(_rt.gui.window, _window_iconify_callback);

    LOG_INFO("Set editor window context.\n");
    //printf("[engine]: Set editor window context.\n");

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG_API_FATAL("glad", "Failed to initialize.\n");
        //fprintf(stderr, "[glad]: Failed to initialize.\n");
        glfwTerminate();
        return 0;
    }

    LOG_API_INFO("glad", "Initialized loader successfully.\n");
    //printf("[engine]: Initialized glad loader.\n");

    return 1;
}


void oce_gui_loop()
{

    oce_shader shader_unlit = oce_shader_load_vf("unlit");
    oce_shader shader_uv1 = oce_shader_load_vf("editor/wireframe_unlit");

    oce_model_load_shader(&_rt.loaded_scene.model_table[1590], shader_unlit);
    oce_model loaded_model = _rt.loaded_scene.model_table[1590];

    LOG_INFO("Loading model %u\n", loaded_model.attribute_flags);
    //printf("Loading model %u\n", loaded_model.attribute_flags);

    //Vsync
    glfwSwapInterval(1);
    glfwShowWindow(_rt.gui.window);
    oce_camera_update_size(&_rt.viewport_camera, 1280, 720);
    //_fb_resize_callback(_rt.gui.window, 1280, 720);

    glfwSetTime(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    double delta_time = 0;
    double last_frame = 0;

    while(!glfwWindowShouldClose(_rt.gui.window))
    {

        glfwPollEvents();

        double _time = glfwGetTime();

        delta_time = _time - last_frame;
        last_frame = _time;

        glClearColor(0.1f, 0.1f, 0.11f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        oce_process_input(delta_time);

        //float dist = -(float)_time;
        //float pos[3] = {dist, dist, dist};
        //glm_vec3_make(pos, _rt.viewport_camera.position_ws);

        //glm_mat4_identity(_rt.viewport_camera.view_matrix);
        //glm_mat4_make(pos, _rt.viewport_camera.view_matrix);

        //glm_mat4_identity(_rt.viewport_camera.view_matrix);

        glm_lookat(_rt.viewport_camera.position_ws, GLM_VEC3_ZERO, GLM_YUP, _rt.viewport_camera.view_matrix);

        glUseProgram(shader_uv1.program);

        GLuint view_loc = glGetUniformLocation(shader_uv1.program, "view");
        GLuint proj_loc = glGetUniformLocation(shader_uv1.program, "proj");

        glUniformMatrix4fv(view_loc, 1, GL_FALSE, _rt.viewport_camera.view_matrix[0]);
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, _rt.viewport_camera.perspective_matrix[0]);

        for(uint32_t i = 0; i < _rt.loaded_scene.model_count; i++)
        {
            oce_model model = _rt.loaded_scene.model_table[i];
            if(model.gl_vertArray == 0 || model.vertex.gl_buffer == 0 || model.index.gl_buffer == 0)
                continue;

            glBindVertexArray(model.gl_vertArray);
            glDrawElements(GL_TRIANGLES, model.index.count, GL_UNSIGNED_INT, 0);
        }


        //glBindVertexArray(0);

        //oce_model_draw(loaded_model);

        glfwSwapBuffers(_rt.gui.window);
    }

    oce_shader_free(&shader_uv1);
    oce_shader_free(&shader_unlit);
}

void oce_gui_free()
{
    glfwDestroyWindow(_rt.gui.window);
    glfwTerminate();
}
