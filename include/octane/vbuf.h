#pragma once
#include "octane/oct/scene.h"
#include <stdint.h>

typedef struct oct_vertexBuffer oct_vertexBuffer;

struct oct_vertexBuffer
{
    uint32_t vertex_count;

    float* positions;
};

oct_vertexBuffer oct_decode_vertex_buffer(oct_sceneDescriptor scene, uint32_t vstream_index);


void oct_free_vertex_buffer(oct_vertexBuffer* const vbuf);
