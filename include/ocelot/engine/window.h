#pragma once

#include "ocelot/engine/model.h"
#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
//#include <GLFW/glfw3.h>

int ocl_gui_init();

void ocl_gui_loop(oce_model model);

