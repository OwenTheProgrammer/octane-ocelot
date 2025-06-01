#pragma once

#include <stdint.h>
typedef struct oct_vertexBuffer oct_vertexBuffer;

struct oct_vertexBuffer
{
    uint16_t name_key;

    uint32_t count;

    uint32_t stride;

    uint32_t buffer_offset;
};
