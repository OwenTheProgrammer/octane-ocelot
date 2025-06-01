#include "octane/oct.h"
#include "octane/oct_atoms.h"
#include "ocelot/endian.h"
#include "oct_internal.h"
#include "utils.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

const size_t _OCT_MAGIC_SIZE = 8;
const size_t _OCT_HEADER_SIZE = 60;

const char _OCT_MAGIC_LE[8] = {0x29, 0x76, 0x01, 0x45, 0xCD, 0xCC, 0x8C, 0x3F};
const char _OCT_MAGIC_BE[8] = {0x45, 0x01, 0x76, 0x29, 0x3F, 0x8C, 0xCC, 0xCD};

static void _free_string_table(oct_string* const table, uint32_t length)
{
    if(table == NULL)
        return;

    for(uint32_t i = 0; i < length; i++)
    {
        free(table[i].data);
    }

    free(table);
}

static void _free_data_tree(oct_atomNode* const tree, uint32_t length)
{
    if(tree == NULL)
        return;

    for(uint32_t i = 0; i < length; i++)
    {
        free(tree[i].data);
    }

    free(tree);
}


endian_t _oct_magic_to_endian(void* const magic)
{
    if(memcmp(magic, _OCT_MAGIC_LE, _OCT_MAGIC_SIZE) == 0)
        return ENDIAN_LITTLE;

    if(memcmp(magic, _OCT_MAGIC_BE, _OCT_MAGIC_SIZE) == 0)
        return ENDIAN_BIG;

    return ENDIAN_UNKNOWN;
}

void _oct_endian_to_magic(endian_t endian, const char** magic)
{
    switch(endian)
    {
        case ENDIAN_LITTLE:
            memcpy(magic, _OCT_MAGIC_LE, _OCT_MAGIC_SIZE);
            break;

        case ENDIAN_BIG:
            memcpy(magic, _OCT_MAGIC_BE, _OCT_MAGIC_SIZE);
            break;

        default:
            break;
    }
}

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


void oct_print_header(oct_header header)
{
    printf("Oct Header:\n");
    printf("|\tEndian: %s\n", _ENDIAN_PRINT_TABLE[(int)header.endian]);
    printf("|\tString table: %u bytes\n", header.string_table_size);
    printf("|\tData tree: %u bytes\n", header.data_tree_size);
}

void oct_print_atom_header(oct_atomHeader header)
{
    printf("Atom Header:\n");
    printf("|\tType: %s\n", _OCT_ATOM_TYPE_PRINT_TABLE[(int)header.atom_type]);
    printf("|\tHas name: %s\n", header.has_name ? "True" : "False");
    printf("|\tData type: %s\n", _OCT_DATA_TYPE_PRINT_TABLE[(int)header.data_type]);
    printf("|\tLength size: %u B\n", header.length_bytes);
    printf("|\tInt size: %u B\n", header.element_bytes);
    printf("|\tLevel: %u\n", header.tree_level);
}

void oct_print_atom_node(oct_file oct, oct_atomNode node)
{
    printf("Node L%u: \"%s\" %s\n",
           node.header.tree_level,
           oct.string_table[node.st_key].data,
           oct.string_table[node.name_key].data
    );

    printf("|\tType: ");
    switch(node.node_type)
    {
        case OCT_NODE_TYPE_BRANCH:  printf("Branch\n"); break;
        case OCT_NODE_TYPE_STRING:  printf("String\n"); break;
        case OCT_NODE_TYPE_FLOAT:   printf("Float\n"); break;
        case OCT_NODE_TYPE_INT:     printf("Int\n"); break;
        case OCT_NODE_TYPE_UUID:    printf("UUID\n"); break;

        case OCT_NODE_TYPE_STRING_ARRAY:    printf("String[%u]\n", node.elem_count); break;
        case OCT_NODE_TYPE_FLOAT_ARRAY:     printf("Float[%u]\n", node.elem_count); break;
        case OCT_NODE_TYPE_INT_ARRAY:       printf("Int[%u]\n", node.elem_count); break;
        case OCT_NODE_TYPE_BINARY:          printf("Binary[%u]\n", node.elem_count); break;
        default:
            printf("???\n");
            break;
    }

    if(node.elem_count == 1)
    {
        switch(node.node_type)
        {
            case OCT_NODE_TYPE_STRING:
                printf("|\tString[%u]: \"%s\"\n", 
                       *(uint16_t*)node.data, 
                       oct.string_table[*(uint16_t*)node.data].data
                );
                break;

            case OCT_NODE_TYPE_FLOAT:
                printf("|\tFloat: %f\n", *(float*)node.data);
                break;

            case OCT_NODE_TYPE_INT:
                printf("|\tInt: %i\n", *(int32_t*)node.data);
                break;

            case OCT_NODE_TYPE_BINARY:
                printf("|\tBinary[%u]:\n", node.elem_count);
                phex_u8_array(node.data, node.elem_count);
                printf("-----------\n");
                break;

            case OCT_NODE_TYPE_UUID:
                printf("|\tUUID: ");
                phex_u8_array(node.data, node.elem_count);
                break;

            default:
                break;
        }
    }
    else if(node.elem_count > 1)
    {
        for(uint32_t i = 0; i < node.elem_count; i++)
        {
            switch(node.node_type)
            {
                case OCT_NODE_TYPE_STRING_ARRAY:
                    printf("|\t%u: String[%u]: \"%s\"\n",
                           i,
                           ((uint16_t*)node.data)[i],
                           oct.string_table[((uint16_t*)node.data)[i]].data
                    );
                    break;

                case OCT_NODE_TYPE_FLOAT_ARRAY:
                    printf("| %u: %f\n", i, ((float*)node.data)[i]);
                    break;

                case OCT_NODE_TYPE_INT_ARRAY:
                    printf("| %u: %i\n", i, ((int32_t*)node.data)[i]);
                    break;

                default:
                    break;
            }
        }
    }

    printf("--------------\n");
}

void oct_print_atom_index_stream(oct_file oct, oct_indexStreamAtom atom)
{
    printf("IndexStream %s\n", oct.string_table[atom.name_key].data);
    printf("|\tLength: %u\n", atom.length);
    printf("|\tBufferReference: %u\n", atom.buffer_reference);
    printf("|\tBufferOffset: %u\n", atom.buffer_offset);
    printf("|\tBufferPrimitive: Triangles\n");
}


void oct_file_free(oct_file* const oct)
{
    if(oct == NULL)
        return;


    _free_string_table(oct->string_table, oct->string_table_length);

    _free_data_tree(oct->data_tree, oct->data_tree_length);

}


const char* _OCT_ATOM_TYPE_PRINT_TABLE[OCT_ATOM_TYPE_MAX] =
{
    "Unknown",
    "Container",
    "List",
    "Element"
};

const char* _OCT_DATA_TYPE_PRINT_TABLE[OCT_DATA_TYPE_MAX] =
{
    "None",
    "String",
    "Float",
    "Int",
    "Binary"
};
