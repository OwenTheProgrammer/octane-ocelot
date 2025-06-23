#pragma once
#include "octane/oct/scene_descriptor.h"
#include <stdint.h>

typedef struct oct_vertexBuffer oct_vertexBuffer;

struct oct_vertexBuffer
{
    uint32_t vertex_count;

    float* positions;
};

oct_vertexBuffer oct_load_vertex_buffer(oct_sceneDescriptor scene, uint32_t index);


void oct_free_vertex_buffer(oct_vertexBuffer* const vbuf);
