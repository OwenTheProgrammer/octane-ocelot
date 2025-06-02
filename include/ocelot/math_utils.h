#pragma once
#include "math_types.h"

/* Converts radians to degrees */
float rad_to_deg(float rad);

/* Converts degrees to radians */
float deg_to_rad(float deg);

/* Computes the cotantgent of x */
float cotf(float x);


vec3f vec3f_negate(vec3f v);


mat3x3f mat3x3f_transpose(mat3x3f m);

mat4x4f mat4x4f_transpose(mat4x4f m);


mat3x3f mat3x3f_rotate_x(float rad);

mat3x3f mat3x3f_rotate_y(float rad);

mat3x3f mat3x3f_rotate_z(float rad);

mat3x3f mat3x3_mult_mat3x3(mat3x3f lhs, mat3x3f rhs);

vec3f mat3x3_mult_vec3(mat3x3f m, vec3f v);

mat4x4f mat4x4f_set_3x3(mat4x4f m, mat3x3f sub);


/* https://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation#From_a_quaternion_to_an_orthogonal_matrix */
mat3x3f quaternion_to_rotor3x3f(quaternion q);

/* Combines the RotationScale part of a 3x3 matrix and the Translation part of a vec3f into a TRS matrix */
mat4x4f mat4x4f_combine_T_RS(mat3x3f rs, vec3f t);

/* Combines a quaternion and translation vector into a 4x4 matrix */
mat4x4f mat4x4f_combine_Q_T(quaternion q, vec3f t);
