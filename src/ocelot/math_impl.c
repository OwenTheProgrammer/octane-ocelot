#include "ocelot/math_types.h"
#include "ocelot/math_utils.h"
#include <math.h>

static const float _180_OVER_PI = 57.2957795131f;
static const float _PI_OVER_180 = 0.01745329251f;

mat3x3f mat3x3f_identity()
{
    return (mat3x3f)
    {
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    };
}

mat4x4f mat4x4f_identity()
{
    return (mat4x4f)
    {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
}

quaternion quaternion_identity()
{
    return (quaternion)
    {
        .i = 0,
        .j = 0,
        .k = 0,
        .r = 1
    };
}

float rad_to_deg(float rad)
{
    return _180_OVER_PI * rad;
}

float deg_to_rad(float deg)
{
    return _PI_OVER_180 * deg;
}

float cotf(float x)
{
    return cosf(x) / sinf(x);
}


vec3f vec3f_negate(vec3f v)
{
    return (vec3f){-v.x, -v.y, -v.z};
}


mat3x3f mat3x3f_transpose(mat3x3f m)
{
    return (mat3x3f)
    {
        m.m00, m.m10, m.m20,
        m.m01, m.m11, m.m21,
        m.m02, m.m12, m.m22
    };
}

mat4x4f mat4x4f_transpose(mat4x4f m)
{
    return (mat4x4f)
    {
        m.m00, m.m10, m.m20, m.m30,
        m.m01, m.m11, m.m21, m.m31,
        m.m02, m.m12, m.m22, m.m32,
        m.m03, m.m13, m.m23, m.m33
    };
}


mat3x3f mat3x3f_rotate_x(float rad)
{
    float s = sinf(rad);
    float c = cosf(rad);
    return (mat3x3f)
    {
        1, 0,  0,
        0, c, -s,
        0, s,  c
    };
}

mat3x3f mat3x3f_rotate_y(float rad)
{
    float s = sinf(rad);
    float c = cosf(rad);
    return (mat3x3f)
    {
         c, 0, s,
         0, 1, 0,
        -s, 0, c
    };
}


mat3x3f mat3x3f_rotate_z(float rad)
{
    float s = sinf(rad);
    float c = cosf(rad);
    return (mat3x3f)
    {
        c, -s, 0,
        s,  c, 0,
        0,  0, 1
    };
}


mat3x3f mat3x3_mult_mat3x3(mat3x3f lhs, mat3x3f rhs)
{
    mat3x3f r = (mat3x3f){0};

    r.m00 = lhs.m00 * rhs.m00 + lhs.m01 * rhs.m10 + lhs.m02 * rhs.m20;
    r.m01 = lhs.m00 * rhs.m01 + lhs.m01 * rhs.m11 + lhs.m02 * rhs.m21;
    r.m02 = lhs.m00 * rhs.m02 + lhs.m01 * rhs.m12 + lhs.m02 * rhs.m22;

    r.m10 = lhs.m10 * rhs.m00 + lhs.m11 * rhs.m10 + lhs.m12 * rhs.m20;
    r.m11 = lhs.m10 * rhs.m01 + lhs.m11 * rhs.m11 + lhs.m12 * rhs.m21;
    r.m12 = lhs.m10 * rhs.m02 + lhs.m11 * rhs.m12 + lhs.m12 * rhs.m22;

    r.m20 = lhs.m20 * rhs.m00 + lhs.m21 * rhs.m10 + lhs.m22 * rhs.m20;
    r.m21 = lhs.m20 * rhs.m01 + lhs.m21 * rhs.m11 + lhs.m22 * rhs.m21;
    r.m22 = lhs.m20 * rhs.m02 + lhs.m21 * rhs.m12 + lhs.m22 * rhs.m22;

    return r;
}

vec3f mat3x3_mult_vec3(mat3x3f m, vec3f v)
{
    vec3f r = (vec3f){0};
    r.x = m.m00 * v.x + m.m01 * v.y + m.m02 * v.z;
    r.y = m.m10 * v.x + m.m11 * v.y + m.m12 * v.z;
    r.z = m.m20 * v.x + m.m21 * v.y + m.m22 * v.z;

    return r;
}


mat4x4f mat4x4f_set_3x3(mat4x4f m, mat3x3f sub)
{
    return (mat4x4f)
    {
        sub.m00, sub.m01, sub.m02, m.m03,
        sub.m10, sub.m11, sub.m12, m.m13,
        sub.m20, sub.m21, sub.m22, m.m23,
        m.m30, m.m31, m.m32, m.m33
    };
}



mat3x3f quaternion_to_rotor3x3f(quaternion q)
{
    //a + bI + cJ + dK
    float a = q.r, b = q.i, c = q.j, d = q.k;
    float a2 = a*a, b2 = b*b, c2 = c*c, d2 = d*d;

    return (mat3x3f)
    {
        a2+b2-c2-d2, 2*(b*c-a*d), 2*(b*d+a*c),
        2*(b*c+a*d), a2-b2+c2-d2, 2*(c*d-a*b),
        2*(b*d-a*c), 2*(c*d+a*b), a2-b2-c2+d2
    };
}


mat4x4f mat4x4f_combine_T_RS(mat3x3f rs, vec3f t)
{
    return (mat4x4f)
    {
        rs.m00, rs.m01, rs.m02, t.x,
        rs.m10, rs.m11, rs.m12, t.y,
        rs.m20, rs.m21, rs.m22, t.z,
        0.0,    0.0,    0.0,    1.0
    };
}

mat4x4f mat4x4f_combine_Q_T(quaternion q, vec3f t)
{
    mat3x3f qr = quaternion_to_rotor3x3f(q);
    return mat4x4f_combine_T_RS(qr, t);
}
