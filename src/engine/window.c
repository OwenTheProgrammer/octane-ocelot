#include "ocelot/engine/window.h"
#include "ocelot/engine_state.h"
//#include <GL/gl.h>

//#define GLFW_INCLUDE_NONE
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
#include <stdio.h>

static void error_callback(int error, const char* msg)
{
    fprintf(stderr, "GLFW: error %i \"%s\"\n", error, msg);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void fb_resize_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


int ocl_gui_init()
{
    glfwSetErrorCallback(error_callback);

    if(!glfwInit())
        return 0;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    ocl_rtState.window = glfwCreateWindow(1280, 720, "Ocelot", NULL, NULL);

    if(!ocl_rtState.window)
    {
        //fprintf(stderr, "Failed to initialize window.\n");
        glfwTerminate();
        return 0;
    }


    glfwMakeContextCurrent(ocl_rtState.window);

    glfwSetKeyCallback(ocl_rtState.window, key_callback);
    glfwSetFramebufferSizeCallback(ocl_rtState.window, fb_resize_callback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fprintf(stderr, "Failed to initialize glad.\n");
        glfwDestroyWindow(ocl_rtState.window);
        glfwTerminate();
        return 0;
    }


    return 1;
}

void ocl_gui_loop()
{
    while(!glfwWindowShouldClose(ocl_rtState.window))
    {

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(ocl_rtState.window);
        glfwPollEvents();
    }

    glfwDestroyWindow(ocl_rtState.window);
    glfwTerminate();
}

