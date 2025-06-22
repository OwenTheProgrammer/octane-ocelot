#pragma once
#include "octane/oct/scene.h"
#include <stdint.h>

typedef struct oct_indexBuffer  oct_indexBuffer;

struct oct_indexBuffer
{
    uint32_t index_count;

    uint32_t* indices;
};


oct_indexBuffer oct_decode_index_buffer(oct_sceneDescriptor scene, uint32_t index);

void oct_free_index_buffer(oct_indexBuffer* const ibuf);
