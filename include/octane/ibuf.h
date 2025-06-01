#pragma once
#include "byteconv/endian.h"
#include "ocelot/dbuf.h"
#include <stdint.h>
#include <stdio.h>

typedef struct oct_indexBuffer  oct_indexBuffer;

struct oct_indexBuffer
{
    endian_t endian;

    uint32_t stride;

    ocl_dbuf data;

    char file_name[FILENAME_MAX];
};

oct_indexBuffer oct_load_ibuf(const char* filepath, uint32_t stride, endian_t endian);

void oct_free_ibuf(oct_indexBuffer* const buffer);
