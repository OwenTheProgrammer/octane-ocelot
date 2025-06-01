#include "octane/ibuf.h"
#include "byteconv/endian.h"
#include "ocelot/dbuf.h"
#include <stdint.h>
#include <stdlib.h>


oct_indexBuffer oct_load_ibuf(const char* filepath, uint32_t stride, endian_t endian)
{
    oct_indexBuffer ibuf = (oct_indexBuffer){0};

    ibuf.stride = stride;
    ibuf.endian = endian;
    ibuf.data = ocl_dbuf_load(filepath);

    return ibuf;
}


void oct_free_ibuf(oct_indexBuffer* const buffer)
{
    if(buffer != NULL)
    {
        ocl_dbuf_free(&buffer->data);
        *buffer = (oct_indexBuffer){0};
    }
}
