#pragma once
#include "data/dbuf.h"
#include "octane/oct/scene_descriptor.h"
#include <stdint.h>

typedef struct oct_indexBuffer  oct_indexBuffer;

struct oct_indexBuffer
{
    uint32_t index_count;

    uint32_t* indices;
};

oct_indexBuffer oct_load_index_buffer(dbuf* const ibuf, oct_sceneDescriptor desc, uint32_t stride, uint32_t pool_index);

void oct_free_index_buffer(oct_indexBuffer* const ibuf);
