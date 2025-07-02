#include "internal.h"
#include "octane/oct/enums.h"
#include "octane/vbuf/vbuf.h"
#include <stddef.h>

const struct _attribInfo _ATTRIBUTE_INFO[OCL_VERTEX_ELEMENT_TYPE_MAX] =
{
    (struct _attribInfo)
    {
        .type = OCL_VERTEX_ELEMENT_TYPE_POSITION,
        .flag = OCL_VERTEX_ELEMENT_FLAG_POSITION,
        .stride = sizeof(float) * 3,
        .src_offset = offsetof(oct_vertex, position),
        .component_type = GL_FLOAT,
        .component_count = 3,
    },
    (struct _attribInfo)
    {
        .type = OCL_VERTEX_ELEMENT_TYPE_UV1,
        .flag = OCL_VERTEX_ELEMENT_FLAG_UV1,
        .stride = sizeof(float) * 2,
        .src_offset = offsetof(oct_vertex, uv1),
        .component_type = GL_FLOAT,
        .component_count = 2,
    },
    (struct _attribInfo)
    {
        .type = OCL_VERTEX_ELEMENT_TYPE_TANGENT,
        .flag = OCL_VERTEX_ELEMENT_FLAG_TANGENT,
        .stride = sizeof(float) * 3,
        .src_offset = offsetof(oct_vertex, tangent),
        .component_type = GL_FLOAT,
        .component_count = 3,
    },
    (struct _attribInfo)
    {
        .type = OCL_VERTEX_ELEMENT_TYPE_NORMAL,
        .flag = OCL_VERTEX_ELEMENT_FLAG_NORMAL,
        .stride = sizeof(float) * 3,
        .src_offset = offsetof(oct_vertex, normal),
        .component_type = GL_FLOAT,
        .component_count = 3,
    },
    (struct _attribInfo)
    {
        .type = OCL_VERTEX_ELEMENT_TYPE_BINORMAL,
        .flag = OCL_VERTEX_ELEMENT_FLAG_BINORMAL,
        .stride = sizeof(float) * 3,
        .src_offset = offsetof(oct_vertex, binormal),
        .component_type = GL_FLOAT,
        .component_count = 3,
    },
    (struct _attribInfo)
    {
        .type = OCL_VERTEX_ELEMENT_TYPE_LIGHTMAP_UV,
        .flag = OCL_VERTEX_ELEMENT_FLAG_LIGHTMAP_UV,
        .stride = sizeof(float) * 2,
        .src_offset = offsetof(oct_vertex, lightmap_uv),
        .component_type = GL_FLOAT,
        .component_count = 2,
    },
    (struct _attribInfo)
    {
        .type = OCL_VERTEX_ELEMENT_TYPE_COLOR_ADD,
        .flag = OCL_VERTEX_ELEMENT_FLAG_COLOR_ADD,
        .stride = sizeof(uint8_t) * 4,
        .src_offset = offsetof(oct_vertex, color_add),
        .component_type = GL_UNSIGNED_BYTE,
        .component_count = 4,
    },
    (struct _attribInfo)
    {
        .type = OCL_VERTEX_ELEMENT_TYPE_COLOR_OCCL,
        .flag = OCL_VERTEX_ELEMENT_FLAG_COLOR_OCCL,
        .stride = sizeof(uint8_t) * 4,
        .src_offset = offsetof(oct_vertex, color_occlusion),
        .component_type = GL_UNSIGNED_BYTE,
        .component_count = 4,
    },
    (struct _attribInfo)
    {
        .type = OCL_VERTEX_ELEMENT_TYPE_VERTEX_BAKED,
        .flag = OCL_VERTEX_ELEMENT_FLAG_VERTEX_BAKED,
        .stride = sizeof(uint8_t) * 4,
        .src_offset = offsetof(oct_vertex, vertex_baked),
        .component_type = GL_UNSIGNED_BYTE,
        .component_count = 4,
    }
};
