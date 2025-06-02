#pragma once
#include "ocelot/dbuf.h"
#include "octane/oct_scene.h"
#include "types/vector.h"
#include <stdint.h>

typedef struct oct_vertexBuffer oct_vertexBuffer;

struct oct_vertexBuffer
{
    uint32_t vertex_count;

    vec3f* positions;
};

oct_vertexBuffer oct_decode_vertex_buffer(ocl_dbuf* const vbuf, oct_sceneDescriptor scene, uint32_t vstream_index);


void oct_free_vertex_buffer(oct_vertexBuffer* const vbuf);
