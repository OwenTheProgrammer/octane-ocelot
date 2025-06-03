#pragma once
#include "cglm/types-struct.h"
#include <stdint.h>
#include <cglm/quat.h>
#include <cglm/vec3.h>
#include <cglm/mat4.h>

typedef struct oce_cameraDescriptor oce_cameraDescriptor;
typedef struct oce_camera           oce_camera;

struct oce_cameraDescriptor
{
    float near_plane;
    float far_plane;
    float fov_rad;

    uint32_t screen_width;
    uint32_t screen_height;
};

struct oce_camera
{
    oce_cameraDescriptor desc;

    /* Camera position in WorldSpace */
    vec3s position_ws;

    /* Camera rotation in WorldSpace */
    versor rotation_ws;

    /* View matrix (Column Major) */
    mat4s view_matrix;

    /* Perspective matrix (Column Major) */ 
    mat4s perspective_matrix;
};

oce_camera oce_camera_init(oce_cameraDescriptor desc);

void oce_camera_update_matrices(oce_camera* const camera);


void oce_camera_update_size(oce_camera* const camera, unsigned int width, unsigned int height);

