#include "ocelot/engine/camera.h"
#include "cglm/struct/affine.h"
#include "cglm/struct/cam.h"
#include "cglm/struct/mat4.h"
#include "cglm/struct/vec3.h"
#include "cglm/types-struct.h"
#include <cglm/mat3.h>
#include <cglm/mat4.h>
#include <cglm/cam.h>

oce_camera oce_camera_init(oce_cameraDescriptor desc)
{
    oce_camera camera = (oce_camera){0};

    camera.desc = desc;
    glm_quat_identity(camera.rotation_ws);

    camera.view_matrix = glms_mat4_identity();

    double aspect = (double)desc.screen_width / (double)desc.screen_height;
    camera.perspective_matrix = glms_perspective(desc.fov_rad, (float)aspect, desc.near_plane, desc.far_plane);
 
    return camera;
}

void oce_camera_update_matrices(oce_camera* const camera)
{
    vec3s nt = glms_vec3_negate(camera->position_ws);
    camera->view_matrix = glms_translate_make(nt);
}

void oce_camera_update_size(oce_camera* const camera, unsigned int width, unsigned int height)
{
    camera->desc.screen_width = width;
    camera->desc.screen_height = height;

    double aspect = (double)width / (double)height;
    camera->perspective_matrix = glms_perspective_resize(camera->perspective_matrix, (float)aspect);
    //camera->perspective_matrix = glms_perspective(camera->desc.fov_rad, (float)aspect, camera->desc.near_plane, camera->desc.far_plane);

    //camera->perspective_matrix = mat4x4f_identity();
    //camera->perspective_matrix = oce_compute_perspective_matrix(camera->desc);
}

