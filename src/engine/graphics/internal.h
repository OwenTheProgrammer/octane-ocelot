#pragma once

#include "octane/oct/enums.h"
#include <stdint.h>
#include <glad/glad.h>

struct _attribInfo
{
    /* The type of this attribute */
    ocl_vertexElementType type;

    /* The flag of this attribute */
    ocl_vertexElementFlag flag;

    /* The stride of the attribute */
    uint32_t stride;

    /* The offset within the `oct_vertex` struct */
    uint32_t src_offset;

    /* The OpenGL attribute component type */
    GLuint component_type;

    /* The OpenGL attribute component count per attribute */
    GLuint component_count;

    /* The OpenGL attribute layout location in the shader */
    GLuint layout_location;
};

extern const struct _attribInfo _ATTRIBUTE_INFO[OCL_VERTEX_ELEMENT_TYPE_MAX];
