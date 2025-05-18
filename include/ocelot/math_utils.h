#pragma once

#include <stdint.h>
#include <stddef.h>

typedef struct vec2f        vec2f;
typedef struct vec3f        vec3f;
typedef struct vec4f        vec4f;

typedef struct mat3x3       mat3x3;
typedef struct mat4x4       mat4x4;

typedef struct quaternion   quaternion;

struct vec2f
{
    float x, y;
};

struct vec3f
{
    float x, y, z;
};

struct vec4f
{
    float x, y, z, w;
};

/* Row Major: m[row][column] */
struct mat3x3
{
    float m00, m01, m02;
    float m10, m11, m12;
    float m20, m21, m22;
};

/* Row Major: m[row][column] */
struct mat4x4
{
    float m00, m01, m02, m03;
    float m10, m11, m12, m13;
    float m20, m21, m22, m23;
    float m30, m31, m32, m33;
};

struct quaternion
{
    float x, y, z, w;
};

/* True if a is approximately equal to b */
int approxf(float a, float b);

/* Converts radians to degrees */
float rad2deg(float rad);

/* Converts degrees to radians */
float deg2rad(float deg);

/* Converts a vec3f of radian values to a vec3f of degree values */
vec3f rad2deg_vec3f(vec3f v);

/* Converts a vec3f of degree values to a vec3f of radian values */
vec3f deg2rad_vec3f(vec3f v);

// == VECTOR ALGEBRA == //

/* Computes the dot product of two vec2fs */
float vec2f_dot(vec2f a, vec2f b);

/* Computes the dot product of two vec3fs */
float vec3f_dot(vec3f a, vec3f b);

/* Computes the (Euclidean) length of a vec2f */
float vec2f_magnitude(vec2f v);

/* Computes the (Euclidean) length of a vec3f */
float vec3f_magnitude(vec3f v);

// == QUATERNION FUNCTIONS == //

/* Returns the quaternion identity, representing no rotation */
quaternion quaternion_identity();

/* Converts a quaternion into (pan/yaw/z) (tilt/pitch/y) (roll/x) radians */
/* https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles#Quaternion_to_Euler_angles_(in_3-2-1_sequence)_conversion */
vec3f quaternion_to_euler_radians(quaternion q);

/* Converts a quaternion into (pan/yaw/z) (tilt/pitch/y) (roll/x) degrees */
vec3f quaternion_to_euler_degrees(quaternion q);

/* Converts a quaternion into an orthonormalized 3x3 rotation matrix */
/* https://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation#From_a_quaternion_to_an_orthogonal_matrix */
mat3x3 quaternion_to_rotor3x3(quaternion q);

/* Combines a quaternion and a translation vector into a 4x4 matrix */
mat4x4 mat4x4_combine_Q_T(quaternion r, vec3f t);

// == MATRIX FUNCTIONS == //

/* Returns the 3x3 identity matrix (the do nothing matrix) */
mat3x3 mat3x3_identity();

/* Returns the 4x4 identity matrix (the do nothing matrix) */
mat4x4 mat4x4_identity();

/* Constructs a 3x3 rotation matrix from (x, y, z) (pan, tilt, roll) radians */
mat3x3 eulers_to_rotor3x3(vec3f euler_rad);

/* Calculates the angles in radians of a normalized rotation matrix */
/* page 5: https://eecs.qmul.ac.uk/~gslabaugh/publications/euler.pdf */
vec3f mat3x3_get_rotation(mat3x3 m);

/* Gets the top left 3x3 submatrix representing [R]otation and [S]cale */
mat3x3 mat4x4_get_RS_submatrix(mat4x4 m);

/* Combines the [R]otation[S]cale part of a 3x3 matrix and the [T]ranslation part of a vec3f into a TRS matrix */
mat4x4 mat4x4_combine_T_RS(mat3x3 rs, vec3f t);

/* Transposes a 3x3 matrix, swapping the columns and rows indices */
mat3x3 mat3x3_transpose(mat3x3 m);

/* Transposes a 4x4 matrix, swapping the columns and rows indices */
mat4x4 mat4x4_transpose(mat4x4 m);

/* Calculates the scale part of a 3x3 matrix, returned as a vec3f */
vec3f mat3x3_get_scale(mat3x3 m);

/* Calculates the scale part of a 4x4 matrix, returned as a vec3f */
vec3f mat4x4_get_scale(mat4x4 m);


void print_vec3f(vec3f v);
void print_vec3f_prefixed(vec3f v, const char* prefix);

void print_mat3x3(mat3x3 m);
void print_mat4x4(mat4x4 m);
