#include "ocelot/dbuf.h"
#include "octane/oct_atoms.h"
#include "octane/vbuf.h"
#include "types/vector.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static uint32_t _find_element_of_type(oct_vertexStreamAtom vstream, oct_vstreamElementName elem_type)
{
    for(uint32_t i = 0; i < vstream.element_count; i++)
    {
        if(vstream.elements[i].attribute_type == elem_type)
            return i;
    }

    return UINT32_MAX;
}


oct_vertexBuffer oct_decode_vertex_buffer(ocl_dbuf* const vbuf, oct_sceneDescriptor scene, uint32_t vstream_index)
{
    oct_vertexBuffer v = (oct_vertexBuffer){0};

    //Get the VertexStream
    oct_vertexStreamAtom vstream_atom = scene.vstream_pool[vstream_index];

    //Capture the vertex count
    v.vertex_count = vstream_atom.length;

    //Seek to the position
    vbuf->ptr = vstream_atom.buffer_offset;


    if(vstream_atom.element_flags & OCT_VSTREAM_ELEMENT_NAME_POSITION)
    {
        uint32_t pos_idx = _find_element_of_type(vstream_atom, OCT_VSTREAM_ELEMENT_NAME_POSITION);
        vbuf->ptr += vstream_atom.elements[pos_idx].offset;
        uint32_t stride = vstream_atom.width;

        v.positions = calloc(v.vertex_count, sizeof(vec3f));

        uint32_t jump = stride - (4*3);

        //Load each vertex position
        for(uint32_t i = 0; i < v.vertex_count; i++)
        {
            vec3f pos = (vec3f){0};
            uint32_t raw;

            raw = ocl_dbuf_read_u32(vbuf);
            pos.x = *(float*)&raw;

            raw = ocl_dbuf_read_u32(vbuf);
            pos.y = *(float*)&raw;

            raw = ocl_dbuf_read_u32(vbuf);
            pos.z = *(float*)&raw;

            v.positions[i] = pos;

            vbuf->ptr += (size_t)jump;
        }

        //Reset the pointer back to the start of the buffer
        vbuf->ptr = vstream_atom.buffer_offset;
    }

    return v;
}


void oct_free_vertex_buffer(oct_vertexBuffer* const vbuf)
{
    if(vbuf != NULL)
    {
        free(vbuf->positions);
    }
}
