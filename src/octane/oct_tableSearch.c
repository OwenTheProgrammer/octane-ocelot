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

    ant.index_stream_pool = oct_get_st_index_of(oct, "IndexStreamPool");

    ant.index_stream = oct_get_st_index_of(oct, "IndexStream");

    ant.index_buffer_reference = oct_get_st_index_of(oct, "IndexBufferReference");

    ant.index_buffer_offset = oct_get_st_index_of(oct, "IndexBufferOffset");

    ant.index_stream_primitive = oct_get_st_index_of(oct, "IndexStreamPrimitive");

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
    PRINT_ST(ant.index_stream_pool)
    PRINT_ST(ant.index_stream)
    PRINT_ST(ant.index_buffer_reference)
    PRINT_ST(ant.index_buffer_offset)
    PRINT_ST(ant.index_stream_primitive)

}

#undef PRINT_ST
