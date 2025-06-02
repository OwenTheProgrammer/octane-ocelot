#pragma once

typedef struct mat4x4f  mat4x4f;

// indexing here is ROW MAJOR so m[y][x] or m[row][col]
struct mat4x4f
{
    float m00, m01, m02, m03;
    float m10, m11, m12, m13;
    float m20, m21, m22, m23;
    float m30, m31, m32, m33;
};


