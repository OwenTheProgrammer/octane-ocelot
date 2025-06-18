#include "octane/ibuf.h"
#include "data/dbuf.h"
#include "octane/oct_atoms.h"
#include <stdlib.h>


oct_indexBuffer oct_decode_index_buffer(oct_rawDataDescriptor scene, uint32_t index)
{
    oct_indexBuffer buf = (oct_indexBuffer){0};

    //Get the IndexStream
    oct_indexStreamAtom istream_atom = scene.istream_pool[index];

    //Evaluate the amount of indices for this IndexStream
    buf.index_count = istream_atom.length * (sizeof(uint32_t) / scene.ibuf_stride);

    //Seek to the buffer position
    scene.ibuf_file.ptr = istream_atom.buffer_offset;

    //Read the indices
    buf.indices = dbuf_read_uvar_array(&scene.ibuf_file, buf.index_count, scene.ibuf_stride);

    return buf;
}

void oct_free_index_buffer(oct_indexBuffer* const ibuf)
{
    if(ibuf != NULL)
    {
        free(ibuf->indices);
    }
}
