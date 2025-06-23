#pragma once
#include "octane/oct/scene_descriptor.h"
#include <stdint.h>

typedef struct oct_indexBuffer  oct_indexBuffer;

struct oct_indexBuffer
{
    uint32_t index_count;

    uint32_t* indices;
};


oct_indexBuffer oct_load_index_buffer(oct_sceneDescriptor scene, uint32_t index);

void oct_free_index_buffer(oct_indexBuffer* const ibuf);
