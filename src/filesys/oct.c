#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "endian.h"
#include "fs_internal.h"
#include "filesys/oct.h"
#include "filesys/file_io.h"

const size_t _OCT_HEADER_SIZE = 60;

const char _OCT_MAGIC_LE[8] = {0x29, 0x76, 0x01, 0x45, 0xcd, 0xcc, 0x8c, 0x3f};
const char _OCT_MAGIC_BE[8] = {0x45, 0x01, 0x76, 0x29, 0x3f, 0x8c, 0xcc, 0xcd};


static const char* _OCT_ATOM_TYPE_TABLE[OCT_ATOM_TYPE_MAX];
static const char* _OCT_DATA_TYPE_TABLE[OCT_DATA_TYPE_MAX];

endian_t _oct_get_magic_endian(void* magic)
{
    if( memcmp(magic, _OCT_MAGIC_LE, sizeof(_OCT_MAGIC_LE)) == 0 )
        return ENDIAN_LITTLE;

    if( memcmp(magic, _OCT_MAGIC_BE, sizeof(_OCT_MAGIC_BE)) == 0 )
        return ENDIAN_BIG;

    return ENDIAN_UNKNOWN;
}

oct_nodeType _oct_get_atom_node_type(io_dbuf* const buffer, oct_atomNode node)
{
    int atom_type = node.header.atom_type;
    int data_type = node.header.data_type;

    return (oct_nodeType)( (atom_type) | (data_type<<2) );
}

oct_atomHeader _oct_parse_atom_header(uint16_t data)
{
    oct_atomHeader header = (oct_atomHeader){0};

    header.atom_type = (oct_atomType)_OCT_MASK_N(data, 2);
    data >>= 2;

    header.has_name = (bool)_OCT_MASK_N(data, 1);
    data >>= 1;

    header.data_type = (oct_dataType)_OCT_MASK_N(data, 3);
    data >>= 3;

    header.length_size = ((unsigned int)_OCT_MASK_N(data, 2)) + 1;
    data >>= 2;

    header.int_size = ((unsigned int)_OCT_MASK_N(data, 2)) + 1;
    data >>= 2;

    header.level = (unsigned int)_OCT_MASK_N(data, 6);

    return header;
}

void oct_free_file(oct_file* const oct)
{
    if(oct != NULL)
    {
        if(oct->string_table != NULL)
            free(oct->string_table);

        if(oct->data_tree != NULL)
        {
            for(size_t i = 0; i < oct->data_tree_length; i++)
            {
                oct_free_atom_node(&oct->data_tree[i]);
            }

            free(oct->data_tree);
        }

        *oct = (oct_file){0};
    }
}

void oct_free_atom_node(oct_atomNode* const node)
{
    if(node != NULL)
    {
        if(node->node_data != NULL)
            free(node->node_data);

        *node = (oct_atomNode){0};
    }
}

void oct_print_header(oct_header header)
{
    printf("OCT Header:\n");
    printf("|\tEndian: %s\n", _ENDIAN_PRINT_TABLE[header.file_endian]);
    printf("|\tString table size: %u\n", header.string_table_size);
    printf("|\tData tree size: %u\n", header.data_tree_size);
}

void oct_print_atom_header(oct_atomHeader header)
{
    printf("Atom Header:\n");
    printf("|\tType: %s\n", _OCT_ATOM_TYPE_TABLE[(int)header.atom_type]);
    printf("|\tHas name: %s\n", header.has_name ? "True" : "False");
    printf("|\tData type: %s\n", _OCT_DATA_TYPE_TABLE[(int)header.data_type]);
    printf("|\tLength size: %u B\n", header.length_size);
    printf("|\tInt size: %u B\n", header.int_size);
    printf("|\tLevel: %u\n", header.level);
}

void oct_print_atom_node(oct_atomNode node, oct_file* const oct)
{
    printf("Node[%u]: \"%s\" %s\n", node.header.level, oct->string_table[node.st_key].ptr, oct->string_table[node.name_key].ptr);
    printf("|\tType: ");
    switch(node.node_type)
    {
        case OCT_NODE_TYPE_BRANCH: printf("Branch\n"); break;
        case OCT_NODE_TYPE_STRING: printf("String\n"); break;
        case OCT_NODE_TYPE_STRING_ARRAY: printf("String[]\n"); break;
        case OCT_NODE_TYPE_FLOAT: printf("Float\n"); break;
        case OCT_NODE_TYPE_FLOAT_ARRAY: printf("Float[]\n"); break;
        case OCT_NODE_TYPE_INT: printf("Int\n"); break;
        case OCT_NODE_TYPE_INT_ARRAY: printf("Int[]\n"); break;
        case OCT_NODE_TYPE_UUID: printf("UUID\n"); break;
        case OCT_NODE_TYPE_BINARY: printf("Binary\n"); break;
        default:
            printf("???\n");
            break;
    }

    if(node.element_count != 0)
        printf("|\tElement Count: %zu\n", node.element_count);

    for(size_t i = 0; i < node.element_count; i++)
    {
        switch(node.node_type)
        {
            case OCT_NODE_TYPE_STRING:
            case OCT_NODE_TYPE_STRING_ARRAY:
                printf("|\t\"%s\"\n", oct->string_table[ ((uint16_t*)node.node_data)[i] ].ptr);
                break;
            case OCT_NODE_TYPE_FLOAT:
            case OCT_NODE_TYPE_FLOAT_ARRAY:
                printf("|\t%f\n", ((float*)node.node_data)[i]);
                break;
            case OCT_NODE_TYPE_INT:
            case OCT_NODE_TYPE_INT_ARRAY:
                printf("|\t%i\n", ((int32_t*)node.node_data)[i]);
                break;
            case OCT_NODE_TYPE_BINARY:
            case OCT_NODE_TYPE_UUID:
            default:
                printf("|\t\'%c\'\n", ((char*)node.node_data)[i]);
                break;
        }
    }

    printf("------------------\n");
}

static const char* _OCT_ATOM_TYPE_TABLE[OCT_ATOM_TYPE_MAX] =
{
    "Unknown",
    "Container",
    "List",
    "Single"
};
static const char* _OCT_DATA_TYPE_TABLE[OCT_DATA_TYPE_MAX] =
{
    "None",
    "String",
    "Float",
    "Int",
    "Binary"
};
