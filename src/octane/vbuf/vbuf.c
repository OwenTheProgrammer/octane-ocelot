#include "data/dbuf.h"
#include "internal.h"
#include "octane/oct/atoms.h"
#include "octane/oct/enums.h"
#include "octane/oct/scene_descriptor.h"
#include "octane/vbuf/vbuf.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>



static uint32_t _find_element_of_type(oct_vertexStreamAtom vstream, ocl_vertexElementType elem_type)
{
    for(uint32_t i = 0; i < vstream.element_count; i++)
    {
        if(vstream.elements[i].attribute_type == elem_type)
            return i;
    }

    return UINT32_MAX;
}

static void _load_vertex_element(dbuf* const vbuf, oct_elementType type, float* const dst)
{
    switch(type)
    {
        case OCT_ELEMENT_TYPE_FLOAT_ONE:
            _oct_vload_float(vbuf, 1, dst);
            break;

        case OCT_ELEMENT_TYPE_FLOAT_TWO:
            _oct_vload_float(vbuf, 2, dst);
            break;

        case OCT_ELEMENT_TYPE_FLOAT_THREE:
            _oct_vload_float(vbuf, 3, dst);
            break;

        case OCT_ELEMENT_TYPE_FLOAT_FOUR:
            _oct_vload_float(vbuf, 4, dst);
            break;

        case OCT_ELEMENT_TYPE_SHORT_TWO_NORM:
            _oct_vload_short2n(vbuf, dst);
            break;

        case OCT_ELEMENT_TYPE_SHORT_FOUR_NORM:
            _oct_vload_short4n(vbuf, dst);
            break;

        case OCT_ELEMENT_TYPE_PVEC_THREE:
            _oct_vload_pvec3(vbuf, dst);
            break;

        case OCT_ELEMENT_TYPE_HALF_TWO:
            _oct_vload_half2(vbuf, dst);
            break;

        case OCT_ELEMENT_TYPE_HALF_FOUR:
            _oct_vload_half4(vbuf, dst);
            break;

        default:
            return;
    }
}


ocl_vertexElementFlag oct_velement_type_to_flag(ocl_vertexElementType type)
{
    switch(type)
    {
        case OCL_VERTEX_ELEMENT_TYPE_NONE:
            return OCL_VERTEX_ELEMENT_FLAG_NONE;

        case OCL_VERTEX_ELEMENT_TYPE_POSITION:
            return OCL_VERTEX_ELEMENT_FLAG_POSITION;

        case OCL_VERTEX_ELEMENT_TYPE_UV1:
            return OCL_VERTEX_ELEMENT_FLAG_UV1;

        case OCL_VERTEX_ELEMENT_TYPE_TANGENT:
            return OCL_VERTEX_ELEMENT_FLAG_TANGENT;

        case OCL_VERTEX_ELEMENT_TYPE_NORMAL:
            return OCL_VERTEX_ELEMENT_FLAG_NORMAL;

        case OCL_VERTEX_ELEMENT_TYPE_BINORMAL:
            return OCL_VERTEX_ELEMENT_FLAG_BINORMAL;

        case OCL_VERTEX_ELEMENT_TYPE_LIGHTMAP_UV:
            return OCL_VERTEX_ELEMENT_FLAG_LIGHTMAP_UV;

        case OCL_VERTEX_ELEMENT_TYPE_COLOR_ADD:
            return OCL_VERTEX_ELEMENT_FLAG_COLOR_ADD;

        case OCL_VERTEX_ELEMENT_TYPE_COLOR_OCCL:
            return OCL_VERTEX_ELEMENT_FLAG_COLOR_OCCL;

        default:
            return OCL_VERTEX_ELEMENT_FLAG_NONE;
    }
}

oct_vertexBuffer oct_load_vertex_buffer(dbuf* const vbuf, oct_sceneDescriptor scene, uint32_t index)
{
    oct_vertexBuffer v = (oct_vertexBuffer){0};

    //Get the VertexStream
    oct_vertexStreamAtom vstream_atom = scene.vstream_pool[index];

    //Capture the vertex count
    v.vertex_count = vstream_atom.length;
    v.vertices = calloc(v.vertex_count, sizeof(oct_vertex));

    //Load each vertex
    for(uint32_t i = 0; i < v.vertex_count; i++)
    {
        //Seek to the beginning of the current vertex
        uint32_t base_offset = vstream_atom.buffer_offset + i * vstream_atom.width;

        oct_vertex vert = (oct_vertex){0};

        for(uint32_t j = 0; j < vstream_atom.element_count; j++)
        {
            oct_vstreamElementAtom element = vstream_atom.elements[j];

            //Seek to the element
            vbuf->ptr = base_offset + element.offset;

            //Get the attribute type
            ocl_vertexElementType ve_type = element.attribute_type;

            //Get the element type
            oct_elementType data_type = element.type;

            switch(ve_type)
            {
                case OCL_VERTEX_ELEMENT_TYPE_POSITION:
                    _load_vertex_element(vbuf, data_type, vert.position);
                    break;

                case OCL_VERTEX_ELEMENT_TYPE_UV1:
                    _load_vertex_element(vbuf, data_type, vert.uv1);
                    break;

                case OCL_VERTEX_ELEMENT_TYPE_TANGENT:
                    _load_vertex_element(vbuf, data_type, vert.tangent);
                    break;

                case OCL_VERTEX_ELEMENT_TYPE_NORMAL:
                    _load_vertex_element(vbuf, data_type, vert.normal);
                    break;

                case OCL_VERTEX_ELEMENT_TYPE_BINORMAL:
                    _load_vertex_element(vbuf, data_type, vert.binormal);
                    break;

                case OCL_VERTEX_ELEMENT_TYPE_LIGHTMAP_UV:
                    _load_vertex_element(vbuf, data_type, vert.lightmap_uv);
                    break;

                default:
                    break;
            }
        }

        v.vertices[i] = vert;
    }

    v.element_flags = vstream_atom.element_flags;

    return v;
}

uint32_t oct_get_vertex_buffer_stride(ocl_vertexElementFlag flags)
{
    uint32_t size = 0;

    if(flags & OCL_VERTEX_ELEMENT_FLAG_POSITION)
        size += sizeof(float) * 3;

    if(flags & OCL_VERTEX_ELEMENT_FLAG_UV1)
        size += sizeof(float) * 2;

    if(flags & OCL_VERTEX_ELEMENT_FLAG_TANGENT)
        size += sizeof(float) * 3;

    if(flags & OCL_VERTEX_ELEMENT_FLAG_NORMAL)
        size += sizeof(float) * 3;

    if(flags & OCL_VERTEX_ELEMENT_FLAG_BINORMAL)
        size += sizeof(float) * 3;

    if(flags & OCL_VERTEX_ELEMENT_FLAG_LIGHTMAP_UV)
        size += sizeof(float) * 2;

    if(flags & OCL_VERTEX_ELEMENT_FLAG_COLOR_ADD)
        size += sizeof(uint8_t) * 4;

    if(flags & OCL_VERTEX_ELEMENT_FLAG_COLOR_OCCL)
        size += sizeof(uint8_t) * 4;

    if(flags & OCL_VERTEX_ELEMENT_FLAG_VERTEX_BAKED)
        size += sizeof(uint8_t) * 4;

    return size;
}


void oct_free_vertex_buffer(oct_vertexBuffer* const vbuf)
{
    if(vbuf != NULL)
    {
        free(vbuf->vertices);
    }
}
