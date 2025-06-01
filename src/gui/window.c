#include "gui/window.h"
#include "ocelot/engine_state.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
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

int ocl_gui_init()
{
    glfwSetErrorCallback(error_callback);

    if(!glfwInit())
        return 0;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    ocl_rtState.window = glfwCreateWindow(800, 600, "Ocelot", NULL, NULL);

    if(!ocl_rtState.window)
    {
        //fprintf(stderr, "Failed to initialize window.\n");
        glfwTerminate();
        return 0;
    }

    //glfwSetKeyCallback(ocl_rtState.window, key_callback);

    glfwMakeContextCurrent(ocl_rtState.window);
    glfwSwapInterval(1);

    return 1;
}

void ocl_gui_loop()
{
    while(!glfwWindowShouldClose(ocl_rtState.window))
    {
        glfwSwapBuffers(ocl_rtState.window);
        glfwPollEvents();
    }

    glfwDestroyWindow(ocl_rtState.window);
    glfwTerminate();
}

