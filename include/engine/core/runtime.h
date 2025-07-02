#pragma once

#include "engine/core/scene.h"
#include "engine/graphics/camera.h"
#include "io/filepath.h"
#include <GLFW/glfw3.h>
typedef struct oce_engineRuntime    oce_engineRuntime;

struct oce_engineRuntime
{
    int initialized;

    struct
    {
        /* The editor working directory / the directory the editor is located in */
        filepath_t work_dir;

        /* The path to the game assets directory */
        filepath_t game_dir;

        /* The path to the engine assets directory */
        filepath_t assets_dir;
    } path;

    struct
    {
        GLFWwindow* window;
    } gui;

    oce_scene loaded_scene;

    oce_camera viewport_camera;
};

/* Global ocelot runtime state */
extern oce_engineRuntime _rt;

/* Initialize the engine runtime state */
void oce_init(const char* argv);

/* Loads a game directory */
void oce_load(const char* assets_dir);

/* Destroy the engine runtime state */
void oce_free();
