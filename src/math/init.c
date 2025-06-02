#include "ocelot/math_types.h"

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
