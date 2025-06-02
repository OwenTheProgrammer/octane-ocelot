#pragma once

typedef struct vec2f    vec2f;
typedef struct vec3f    vec3f;

typedef struct mat3x3f  mat3x3f;
typedef struct mat4x4f  mat4x4f;

typedef struct quaternion   quaternion;


struct vec2f
{
    float x, y;
};

struct vec3f
{
    float x, y, z;
};

struct mat3x3f
{
    float m00, m01, m02;
    float m10, m11, m12;
    float m20, m21, m22;
};

// indexing here is ROW MAJOR so m[y][x] or m[row][col]
struct mat4x4f
{
    float m00, m01, m02, m03;
    float m10, m11, m12, m13;
    float m20, m21, m22, m23;
    float m30, m31, m32, m33;
};

struct quaternion
{
    float i, j, k, r;
};


mat3x3f mat3x3f_identity();

mat4x4f mat4x4f_identity();

quaternion quaternion_identity();
