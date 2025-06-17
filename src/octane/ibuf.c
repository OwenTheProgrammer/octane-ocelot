#include "octane/ibuf.h"
#include "data/dbuf.h"
#include "octane/oct_atoms.h"
#include <stdlib.h>


oct_indexBuffer oct_decode_index_buffer(dbuf* const ibuf, oct_rawDataDescriptor scene, uint32_t stride, uint32_t index)
{
    oct_indexBuffer buf = (oct_indexBuffer){0};

    oct_indexStreamAtom istream_atom = scene.istream_pool[index];

    buf.index_count = istream_atom.length * (sizeof(uint32_t) / stride);

    ibuf->ptr = istream_atom.buffer_offset;

    buf.indices = dbuf_read_uvar_array(ibuf, buf.index_count, stride);

    return buf;
}

void oct_free_index_buffer(oct_indexBuffer* const ibuf)
{
    if(ibuf != NULL)
    {
        free(ibuf->indices);
    }
}
