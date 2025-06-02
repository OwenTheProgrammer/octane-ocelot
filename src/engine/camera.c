#include "ocelot/engine/camera.h"
#include "ocelot/math_types.h"
#include "ocelot/math_utils.h"
#include <math.h>


oce_camera oce_camera_init(oce_cameraDescriptor desc)
{
    oce_camera camera = (oce_camera){0};

    camera.desc = desc;
    camera.rotation_ws = quaternion_identity();

    camera.view_matrix = mat4x4f_identity();
    //camera.perspective_matrix = mat4x4f_identity();
    camera.perspective_matrix = oce_compute_perspective_matrix(desc);

    return camera;
}

void oce_camera_update_matrices(oce_camera* const camera)
{
    // Update the view matrix
    mat3x3f r = quaternion_to_rotor3x3f(camera->rotation_ws);
    vec3f nt = vec3f_negate(camera->position_ws);
    mat4x4f v = mat4x4f_combine_T_RS(mat3x3f_transpose(r), nt);

    //camera->view_matrix = mat4x4f_identity();
    camera->view_matrix = v;
}

void oce_camera_update_size(oce_camera* const camera, unsigned int width, unsigned int height)
{
    camera->desc.screen_width = width;
    camera->desc.screen_height = height;
    camera->perspective_matrix = mat4x4f_identity();
    camera->perspective_matrix = oce_compute_perspective_matrix(camera->desc);
}


mat4x4f oce_compute_perspective_matrix(oce_cameraDescriptor desc)
{
    double aspect = (double)desc.screen_width / (double)desc.screen_height;
    double sf = tanf(desc.fov_rad * 0.5) * desc.near_plane;

    //top bottom left right
    double t = sf;
    double b = -t;
    double r = aspect * sf;
    double l = -r;

    double n = (double)desc.near_plane;
    double f = (double)desc.far_plane;

    float m00 = (2.0 * n) / (r - l);
    float m11 = (2.0 * n) / (t - b);
    float m20 = (r + l) / (r - l);
    float m21 = (t + b) / (t - b);
    float m22 = -(f + n) / (f - n);
    float m23 = -1.0;
    float m32 = -(2.0 * f * n) / (f - n);

    return (mat4x4f)
    {
        m00, 0.0, m20, 0.0,
        0.0, m11, m21, 0.0,
        0.0, 0.0, m22, m32,
        0.0, 0.0, m23, 0.0
    };
}
