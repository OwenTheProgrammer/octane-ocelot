#pragma once

#include <GLFW/glfw3.h>
typedef struct ocl_runtimeState ocl_runtimeState;

struct ocl_runtimeState
{
    GLFWwindow* window;
};

extern ocl_runtimeState ocl_rtState;
