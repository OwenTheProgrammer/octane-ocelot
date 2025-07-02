#pragma once

#include "cglm/types.h"
#include <stdint.h>

typedef struct oce_cameraDescriptor oce_cameraDescriptor;
typedef struct oce_camera           oce_camera;

struct oce_cameraDescriptor
{
    float near_plane;
    float far_plane;
    float fov_rad;

    uint32_t width_px;
    uint32_t height_px;
};

struct oce_camera
{
    oce_cameraDescriptor desc;

    vec3 position_ws;

    versor rotation_ws;

    mat4 view_matrix;

    mat4 perspective_matrix;
};

oce_camera oce_camera_init(oce_cameraDescriptor desc);

void oce_camera_update_size(oce_camera* const cam, unsigned int width, unsigned int height);
