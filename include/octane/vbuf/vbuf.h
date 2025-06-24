#pragma once
#include "octane/oct/enums.h"
#include "octane/oct/scene_descriptor.h"
#include <stdint.h>

typedef struct oct_vertexBuffer oct_vertexBuffer;
typedef struct oct_vertex       oct_vertex;

struct oct_vertexBuffer
{
    uint32_t vertex_count;

    ocl_vertexElementType element_flags;

    oct_vertex* vertices;
};

struct oct_vertex
{
    float position[3];

    float uv1[2];

    float tangent[3];

    float normal[3];

    float binormal[3];

    float lightmap_uv[2];

    uint8_t color_add[4];

    uint8_t color_occlusion[4];

    uint8_t vertex_baked[4];
};

oct_vertexBuffer oct_load_vertex_buffer(oct_sceneDescriptor scene, uint32_t index);


void oct_free_vertex_buffer(oct_vertexBuffer* const vbuf);
