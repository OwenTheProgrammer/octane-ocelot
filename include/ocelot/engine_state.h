#pragma once

#include "ocelot/engine/camera.h"
#include <GLFW/glfw3.h>
typedef struct ocl_runtimeState ocl_runtimeState;

struct ocl_runtimeState
{
    GLFWwindow* window;

    oce_camera viewport_camera;
};

extern ocl_runtimeState ocl_rtState;
