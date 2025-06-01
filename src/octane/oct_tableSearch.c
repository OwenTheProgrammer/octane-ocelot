#include "octane/oct.h"
#include "octane/oct_atoms.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

oct_atomNameTable _oct_ant = (oct_atomNameTable){0};

uint16_t oct_get_st_index_of(oct_file oct, const char* str)
{
    if(str == NULL)
        return 0;

    uint32_t in_len = strlen(str);

    if(in_len == 0)
        return 0;

    for(uint32_t i = 0; i < oct.string_table_length; i++)
    {
        oct_string element = oct.string_table[i];

        if(element.len == in_len && strncmp(str, element.data, element.len) == 0)
            return (uint16_t)i;
    }

    return 0;
}


uint32_t oct_get_atom_node_index(oct_file oct, uint16_t key)
{
    if(key == 0)
        return 0;

    for(uint32_t i = 0; i < oct.data_tree_length; i++)
    {
        oct_atomNode node = oct.data_tree[i];

        if(node.st_key == key)
            return i;
    }

    return 0;
}


oct_atomNameTable oct_create_atom_name_table(oct_file oct)
{
    oct_atomNameTable ant = (oct_atomNameTable){0};

    ant.length = oct_get_st_index_of(oct, "Length");

    ant.width = oct_get_st_index_of(oct, "Width");

    ant.type = oct_get_st_index_of(oct, "Type");

    ant.offset = oct_get_st_index_of(oct, "Offset");

    ant.name = oct_get_st_index_of(oct, "Name");


    ant.index_stream_pool = oct_get_st_index_of(oct, "IndexStreamPool");

    ant.index_stream = oct_get_st_index_of(oct, "IndexStream");

    ant.index_buffer_reference = oct_get_st_index_of(oct, "IndexBufferReference");

    ant.index_buffer_offset = oct_get_st_index_of(oct, "IndexBufferOffset");

    ant.index_stream_primitive = oct_get_st_index_of(oct, "IndexStreamPrimitive");


    ant.vertex_stream_pool = oct_get_st_index_of(oct, "VertexStreamPool");

    ant.vertex_stream = oct_get_st_index_of(oct, "VertexStream");

    ant.vstream_buffer_reference = oct_get_st_index_of(oct, "VertexBufferReference");

    ant.vstream_buffer_offset = oct_get_st_index_of(oct, "VertexBufferOffset");

    ant.vstream_elements = oct_get_st_index_of(oct, "Elements");

    ant.vstream_element = oct_get_st_index_of(oct, "Element");


    ant.vstream_element_name_position = oct_get_st_index_of(oct, "Position");

    ant.vstream_element_name_uv1 = oct_get_st_index_of(oct, "Uv1");

    ant.vstream_element_name_tangent = oct_get_st_index_of(oct, "Tangent");

    ant.vstream_element_name_normal = oct_get_st_index_of(oct, "Normal");

    ant.vstream_element_name_binormal = oct_get_st_index_of(oct, "Binormal");

    ant.vstream_element_name_lightmapUV = oct_get_st_index_of(oct, "lightmapUV");

    return ant;
}

void oct_init_atom_name_table(oct_file oct)
{
    _oct_ant = oct_create_atom_name_table(oct);
}

#define PRINT_ST(x) printf("|\t\"%s\": StringTable[%u]\n", oct.string_table[x].data, x);

void oct_print_atom_name_table(oct_file oct, oct_atomNameTable ant)
{
    printf("AtomNameTable:\n");
    PRINT_ST(ant.length)
    PRINT_ST(ant.width)
    PRINT_ST(ant.type)
    PRINT_ST(ant.offset)
    PRINT_ST(ant.name)

    PRINT_ST(ant.index_stream_pool)
    PRINT_ST(ant.index_stream)
    PRINT_ST(ant.index_buffer_reference)
    PRINT_ST(ant.index_buffer_offset)
    PRINT_ST(ant.index_stream_primitive)

    PRINT_ST(ant.vertex_stream_pool)
    PRINT_ST(ant.vertex_stream)
    PRINT_ST(ant.vstream_buffer_reference)
    PRINT_ST(ant.vstream_buffer_offset)
    PRINT_ST(ant.vstream_elements)
    PRINT_ST(ant.vstream_element)

    PRINT_ST(ant.vstream_element_name_position)
    PRINT_ST(ant.vstream_element_name_uv1)
    PRINT_ST(ant.vstream_element_name_tangent)
    PRINT_ST(ant.vstream_element_name_normal)
    PRINT_ST(ant.vstream_element_name_binormal)
    PRINT_ST(ant.vstream_element_name_lightmapUV)
}

#undef PRINT_ST
