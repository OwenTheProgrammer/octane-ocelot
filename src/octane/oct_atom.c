#include "octane/oct.h"
#include "octane/oct_atoms.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

oct_indexStreamAtom oct_atom_read_index_stream(oct_file oct, uint32_t start_idx, uint32_t end_idx)
{
    oct_indexStreamAtom atom = (oct_indexStreamAtom){0};

    oct_atomNode start_node = oct.data_tree[start_idx];
    uint32_t start_level = start_node.header.tree_level;

    atom.name_key = start_node.name_key;

    for(uint32_t i = start_idx+1; i < end_idx; i++)
    {
        oct_atomNode node = oct.data_tree[i];

        // TODO: Stream primitive is not read
        // TODO: No checks for endian type currently, although not needed

        if(node.st_key == _oct_ant.length)
        {
            atom.length = *(uint32_t*)node.data;
        }
        else if(node.st_key == _oct_ant.index_buffer_reference)
        {
            atom.buffer_reference = *(uint32_t*)node.data;
        }
        else if(node.st_key == _oct_ant.index_buffer_offset)
        {
            atom.buffer_offset = *(uint32_t*)node.data;
        }
        else if(node.st_key == _oct_ant.index_stream_primitive)
        {
            atom.stream_primitive = OCT_PRIMITIVE_TYPE_TRIANGLES;
        }

    }

    return atom;
}


void oct_print_atom_index_stream(oct_file oct, oct_indexStreamAtom atom)
{
    printf("IndexStream %s\n", oct.string_table[atom.name_key].data);
    printf("|\tLength: %u\n", atom.length);
    printf("|\tBufferReference: %u\n", atom.buffer_reference);
    printf("|\tBufferOffset: %u\n", atom.buffer_offset);
    printf("|\tBufferPrimitive: Triangles\n");
}
