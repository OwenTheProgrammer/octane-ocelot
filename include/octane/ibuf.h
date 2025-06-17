#pragma once
#include "data/dbuf.h"
#include "octane/oct_scene.h"
#include <stdint.h>

typedef struct oct_indexBuffer  oct_indexBuffer;

struct oct_indexBuffer
{
    uint32_t index_count;

    uint32_t* indices;
};


oct_indexBuffer oct_decode_index_buffer(dbuf* const ibuf, oct_rawDataDescriptor scene, uint32_t stride, uint32_t index);

void oct_free_index_buffer(oct_indexBuffer* const ibuf);
