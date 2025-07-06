#include "engine/core/controls.h"
#include "GLFW/glfw3.h"
#include "cglm/quat.h"
#include "cglm/types.h"
#include "cglm/vec3.h"
#include "engine/core/runtime.h"


void oce_process_input(double delta_time)
{
    double move_speed = 15.0 * delta_time;

    GLFWwindow* window = _rt.gui.window;

    mat3 view_mat;
    glm_quat_mat3(_rt.viewport_camera.rotation_ws, view_mat);

    vec3 cam_fwd, cam_right, cam_up;
    glm_quat_rotatev(_rt.viewport_camera.rotation_ws, ((vec3){0.0f, 0.0f, -1.0f}), cam_fwd);
    glm_quat_rotatev(_rt.viewport_camera.rotation_ws, ((vec3){1.0f, 0.0f, 0.0f}), cam_right);
    glm_quat_rotatev(_rt.viewport_camera.rotation_ws, ((vec3){0.0f, 1.0f, 0.0f}), cam_up);

    vec3 move_delta;
    glm_vec3_zero(move_delta);

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        glm_vec3_muladds(cam_fwd, move_speed, move_delta);

    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        glm_vec3_mulsubs(cam_right, move_speed, move_delta);

    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        glm_vec3_muladds(cam_right, move_speed, move_delta);

    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        glm_vec3_mulsubs(cam_fwd, move_speed, move_delta);

    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        glm_vec3_mulsubs(cam_up, move_speed, move_delta);

    if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        glm_vec3_muladds(cam_up, move_speed, move_delta);

    glm_vec3_norm(move_delta);

    glm_vec3_add(_rt.viewport_camera.position_ws, move_delta, _rt.viewport_camera.position_ws);
}
