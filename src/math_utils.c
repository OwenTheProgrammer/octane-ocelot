#include "math_utils.h"
#include <stdio.h>

#define __USE_GNU
#include <math.h>
#include <float.h>

mat3x3 mat3x3_identity()
{
    return (mat3x3)
    {
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0
    };
}

mat4x4 mat4x4_identity()
{
    return (mat4x4)
    {
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    };
}

quaternion quaternion_identity()
{
    return (quaternion)
    {
        0.0, 0.0, 0.0, 1.0
    };
}


int approxf(float a, float b)
{
    return (fabsf(a - b) < FLT_EPSILON);
}

float rad2deg(float rad)
{
    //180/pi * radians
    return 57.2957795131f * rad;
}

float deg2rad(float deg)
{
    //pi/180 * degrees
    return 0.01745329251f * deg;
}

vec3f rad2deg_vec3f(vec3f v)
{
    return (vec3f)
    {
        rad2deg(v.x),
        rad2deg(v.y),
        rad2deg(v.z)
    };
}

vec3f deg2rad_vec3f(vec3f v)
{
    return (vec3f)
    {
        deg2rad(v.x),
        deg2rad(v.y),
        deg2rad(v.z)
    };
}



float vec2f_dot(vec2f a, vec2f b)
{
    return a.x*b.x + a.y*b.y;
}

float vec3f_dot(vec3f a, vec3f b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

float vec2f_magnitude(vec2f v)
{
    return sqrtf(v.x*v.x + v.y*v.y);
}

float vec3f_magnitude(vec3f v)
{
    return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
}


vec3f quaternion_to_euler_radians(quaternion q)
{
    float sr_cp = 2 * (q.w*q.x + q.y*q.z);
    float cr_cp = 1-2*(q.x*q.x + q.y*q.y);
    float sy_cp = 2 * (q.w*q.z + q.x*q.y);
    float cy_cp = 1-2*(q.y*q.y + q.z*q.z);

    float roll_x = atan2f(sr_cp, cr_cp);
    float pitch_y = asinf(2*(q.w*q.y - q.x*q.z));
    float yaw_z = atan2f(sy_cp, cy_cp);

    return (vec3f){roll_x, pitch_y, yaw_z};
}

vec3f quaternion_to_euler_degrees(quaternion q)
{
    vec3f euler_rad = quaternion_to_euler_radians(q);
    return rad2deg_vec3f(euler_rad);
}

mat3x3 eulers_to_rotor3x3(vec3f euler_rad)
{
    //roll
    float sr = sinf(euler_rad.x);
    float cr = cosf(euler_rad.x);
    //tilt
    float st = sinf(euler_rad.y);
    float ct = cosf(euler_rad.y);
    //pan
    float sp = sinf(euler_rad.z);
    float cp = cosf(euler_rad.z);

    return (mat3x3)
    {
        cp*ct, cp*st*sr-sp*cr, cp*st*cr+sp*sr,
        sp*ct, sp*st*sr+cp*cr, sp*st*cr-cp*sr,
        -st, ct*sr, ct*cr
    };
}

mat3x3 quaternion_to_rotor3x3(quaternion q)
{
    //xI + yJ + zK + w -> a + bI + cJ + dK
    float a = q.w, b = q.x, c = q.y, d = q.z;
    float a2 = a*a, b2 = b*b, c2 = c*c, d2 = d*d;

    return (mat3x3)
    {
        a2+b2-c2-d2, 2*(b*c-a*d), 2*(b*d+a*c),
        2*(b*c+a*d), a2-b2+c2-d2, 2*(c*d-a*b),
        2*(b*d-a*c), 2*(c*d+a*b), a2-b2-c2+d2
    };
}


mat3x3 mat4x4_get_RS_submatrix(mat4x4 m)
{
    return (mat3x3)
    {
        m.m00, m.m01, m.m02,
        m.m10, m.m11, m.m12,
        m.m20, m.m21, m.m22
    };
}

mat4x4 mat4x4_combine_T_RS(mat3x3 rs, vec3f t)
{
    return (mat4x4)
    {
        rs.m00, rs.m01, rs.m02, t.x,
        rs.m10, rs.m11, rs.m12, t.y,
        rs.m20, rs.m21, rs.m22, t.z,
        0.0,    0.0,    0.0,    1.0
    };
}

mat4x4 mat4x4_combine_Q_T(quaternion r, vec3f t)
{
    mat3x3 qr = quaternion_to_rotor3x3(r);
    return mat4x4_combine_T_RS(qr, t);
}


mat3x3 mat3x3_transpose(mat3x3 m)
{
    return (mat3x3)
    {
        m.m00, m.m10, m.m20,
        m.m01, m.m11, m.m21,
        m.m02, m.m12, m.m22
    };
}
mat4x4 mat4x4_transpose(mat4x4 m)
{
    return (mat4x4)
    {
        m.m00, m.m10, m.m20, m.m30,
        m.m01, m.m11, m.m21, m.m32,
        m.m02, m.m12, m.m22, m.m32,
        m.m03, m.m13, m.m23, m.m33
    };
}


vec3f mat3x3_get_scale(mat3x3 m)
{
    //Length of each column vector
    float sx = vec3f_magnitude((vec3f){m.m00, m.m10, m.m20});
    float sy = vec3f_magnitude((vec3f){m.m01, m.m11, m.m21});
    float sz = vec3f_magnitude((vec3f){m.m02, m.m12, m.m22});
    return (vec3f) { sx, sy, sz };
}

vec3f mat4x4_get_scale(mat4x4 m)
{
    mat3x3 rs = mat4x4_get_RS_submatrix(m);
    return mat3x3_get_scale(rs);
}

vec3f mat3x3_get_rotation(mat3x3 m)
{
    float theta = 0;
    float rho = 0;
    float phi = 0;
    //r31 != +-1
    if(!approxf(fabsf(m.m20), 1.0))
    {
        theta = -asinf(m.m20);
        //1/cos(-arcsin(m.m20))
        float ict = 1.0 / sqrtf(1.0 - m.m20 * m.m20);

        rho = atan2f(m.m21 * ict, m.m22 * ict);
        phi = atan2f(m.m10 * ict, m.m00 * ict);
    }
    else if(m.m20 < 0.0)
    {
        theta = M_PI_2;
        rho = atan2f(m.m01, m.m02);
    }
    else
    {
        theta = -M_PI_2;
        rho = atan2f(-m.m01, -m.m02);
    }

    return (vec3f){rho, theta, phi};
}


void print_vec3f(vec3f v)
{
    printf("Vector3: (x: %.4f, y: %.4f, z: %.4f)\n", v.x, v.y, v.z);
}
void print_vec3f_prefixed(vec3f v, const char* prefix)
{
    printf("Vector3: (%.4f %s, %.4f %s, %.4f %s)\n", v.x, prefix, v.y, prefix, v.z, prefix);
}
void print_mat3x3(mat3x3 m)
{
    printf("Matrix(3x3): ROW-MAJOR\n");
    printf("m0#:\t%.4f, %.4f, %.4f\n", m.m00, m.m01, m.m02);
    printf("m1#:\t%.4f, %.4f, %.4f\n", m.m10, m.m11, m.m12);
    printf("m2#:\t%.4f, %.4f, %.4f\n", m.m20, m.m21, m.m22);
}
void print_mat4x4(mat4x4 m)
{
    printf("Matrix(4x4): ROW-MAJOR\n");
    printf("m0#:\t%.4f, %.4f, %.4f, %.4f\n", m.m00, m.m01, m.m02, m.m03);
    printf("m1#:\t%.4f, %.4f, %.4f, %.4f\n", m.m10, m.m11, m.m12, m.m13);
    printf("m2#:\t%.4f, %.4f, %.4f, %.4f\n", m.m20, m.m21, m.m22, m.m23);
    printf("m3#:\t%.4f, %.4f, %.4f, %.4f\n", m.m30, m.m31, m.m32, m.m33);
}
