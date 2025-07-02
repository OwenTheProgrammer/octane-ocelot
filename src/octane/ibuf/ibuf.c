#include "octane/ibuf/ibuf.h"
#include "data/dbuf.h"
#include "internal.h"
#include "octane/oct/atoms.h"
#include <stdint.h>
#include <stdlib.h>

oct_indexBuffer oct_load_index_buffer(dbuf* const ibuf, oct_sceneDescriptor desc, uint32_t stride, uint32_t pool_index)
{
    oct_indexBuffer buffer = (oct_indexBuffer){0};

    oct_indexStreamAtom* atom = &desc.istream_pool[pool_index];

    //Evaluate the amount of indices for this buffer
    buffer.index_count = _oct_ibuf_get_index_count(atom->length, stride);

    //Seek and read to the buffer data
    ibuf->ptr = atom->buffer_offset;
    buffer.indices = dbuf_read_uvar_array(ibuf, buffer.index_count, stride);

    return buffer;
}

void oct_free_index_buffer(oct_indexBuffer* const ibuf)
{
    if(ibuf != NULL)
    {
        free(ibuf->indices);
    }
}
