#include "engine/graphics/camera.h"
#include "cglm/cam.h"
#include "cglm/mat4.h"
#include "cglm/quat.h"
#include "engine/internal.h"
#include <stdio.h>



oce_camera oce_camera_init(oce_cameraDescriptor desc)
{
    oce_camera cam = (oce_camera){0};

    cam.desc = desc;
    glm_quat_identity(cam.rotation_ws);
    glm_mat4_identity(cam.view_matrix);

    if(desc.width_px != 0 && desc.height_px != 0)
    {
        double aspect = (double)(desc.width_px) / (double)(desc.height_px);
        glm_perspective(desc.fov_rad, (float)aspect, desc.near_plane, desc.far_plane, cam.perspective_matrix);
    }
    else
    {
        glm_mat4_identity(cam.perspective_matrix);
    }

    return cam;
}


void oce_camera_update_size(oce_camera* const cam, unsigned int width, unsigned int height)
{
    int needs_init = cam->desc.width_px == 0 || cam->desc.height_px == 0;

    cam->desc.width_px = width;
    cam->desc.height_px = height;

    if(width != 0 && height != 0)
    {
        double aspect = (double)(width) / (double)(height);

        if(needs_init)
        {
            glm_perspective(
                cam->desc.fov_rad,
                (float)aspect,
                cam->desc.near_plane,
                cam->desc.far_plane,
                cam->perspective_matrix
            );

            LOG_INFO("Reinitialized camera perspective matrix.\n");
            //printf("[engine]: Reinitialized camera perspective matrix.\n");
        }
        else
        {
            LOG_INFO("Resized camera perspective matrix.\n");
            //printf("[engine]: Resized camera perspective matrix.\n");
            glm_perspective_resize(
                (float)aspect,
                cam->perspective_matrix
            );
        }
    }
    else
    {
        LOG_INFO("Unloaded camera perspective matrix.\n");
        //printf("[engine]: Unloaded camera perspective matrix.\n");
        glm_mat4_identity(cam->perspective_matrix);
    }
}
