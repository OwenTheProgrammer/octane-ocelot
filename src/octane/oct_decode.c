#include "oct_internal.h"
#include "octane/oct.h"
#include "ocelot/dbuf.h"
#include "byteconv/endian.h"
#include "utils.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static oct_nodeType _get_node_type(oct_atomHeader header)
{
    int atom_type = header.atom_type;
    int data_type = header.data_type;

    return (oct_nodeType)( (atom_type) | (data_type << 2));
}

static uint32_t _get_atom_array_size(ocl_dbuf* const buffer, uint32_t stride)
{
    if(stride > 4)
    {
        fprintf(stderr, "Atom array stride (%u) can not be greater than 4.\n", stride);
        return 0;
    }

    return ocl_dbuf_read_uvar(buffer, stride);
}

static oct_atomHeader _parse_atom_header(uint16_t data)
{
    oct_atomHeader header = (oct_atomHeader){0};

    header.atom_type = (oct_atomType)NMASK(data, 2);
    data >>= 2;

    header.has_name = (bool)NMASK(data, 1);
    data >>= 1;

    header.data_type = (oct_dataType)NMASK(data, 3);
    data >>= 3;

    header.length_bytes = ((uint32_t)NMASK(data, 2)) + 1;
    data >>= 2;

    header.element_bytes = ((uint32_t)NMASK(data, 2)) + 1;
    data >>= 2;

    header.tree_level = ((uint32_t)NMASK(data, 6));
    //data >>= 6;

    return header;
}

static oct_atomNode _read_atom_node(ocl_dbuf* const buffer, oct_file* const oct)
{
    oct_atomNode node = (oct_atomNode){0};

    node.header = _parse_atom_header(ocl_dbuf_read_u16(buffer));
    node.st_key = ocl_dbuf_read_u16(buffer);

    if(node.header.has_name)
        node.name_key = ocl_dbuf_read_u16(buffer);

    node.node_type = _get_node_type(node.header);

    uint32_t list_stride = node.header.length_bytes;
    uint32_t elem_stride = node.header.element_bytes;

    switch(node.node_type)
    {
        case OCT_NODE_TYPE_STRING:
            node.elem_count = 1;
            node.data = ocl_dbuf_read_u16_array(buffer, node.elem_count);
            break;

        case OCT_NODE_TYPE_STRING_ARRAY:
            node.elem_count = _get_atom_array_size(buffer, list_stride);
            node.data = ocl_dbuf_read_u16_array(buffer, node.elem_count);
            break;

        case OCT_NODE_TYPE_FLOAT:
            node.elem_count = 1;
            node.data = ocl_dbuf_read_u32_array(buffer, node.elem_count);
            break;

        case OCT_NODE_TYPE_FLOAT_ARRAY:
            node.elem_count = _get_atom_array_size(buffer, list_stride);
            node.data = ocl_dbuf_read_u32_array(buffer, node.elem_count);
            break;

        case OCT_NODE_TYPE_INT:
            node.elem_count = 1;
            node.data = ocl_dbuf_read_svar_array(buffer, node.elem_count, elem_stride);
            break;

        case OCT_NODE_TYPE_INT_ARRAY:
            node.elem_count = _get_atom_array_size(buffer, list_stride);
            node.data = ocl_dbuf_read_svar_array(buffer, node.elem_count, elem_stride);
            break;

        case OCT_NODE_TYPE_BINARY:
            node.elem_count = _get_atom_array_size(buffer, list_stride);
            node.data = ocl_dbuf_read_u8_array(buffer, node.elem_count);
            break;

        case OCT_NODE_TYPE_UUID:
            node.elem_count = 16;
            node.data = ocl_dbuf_read_u8_array(buffer, node.elem_count);
            break;

        default:
            break;
    }

    return node;
}

static oct_header _read_header(oct_file oct, ocl_dbuf* const buffer)
{
    oct_header header = (oct_header){0};

    endian_t magic_endian = _oct_magic_to_endian(buffer->data);

    if(magic_endian == ENDIAN_UNKNOWN)
    {
        fprintf(stderr, "Invalid magic!\n");
        return header;
    }

    // Set the endian context to the oct file endian
    endian_set_current(magic_endian);

    header.endian = endian_get_target();

    // Seek forward past the endian section
    ocl_dbuf_advance(buffer, _OCT_MAGIC_SIZE);

    printf("Reading oct file (Endian: %s -> %s)\n",
           _ENDIAN_PRINT_TABLE[(int)endian_get_current()],
           _ENDIAN_PRINT_TABLE[(int)endian_get_target()]
    );

    // Ignore the padding bytes
    ocl_dbuf_advance(buffer, 4);

    // Load the string table size
    header.string_table_size = ocl_dbuf_read_u32(buffer);

    // Load the data tree size
    header.data_tree_size = ocl_dbuf_read_u32(buffer);

    // Ignore the 40 bytes of padding
    ocl_dbuf_advance(buffer, 40);

    return header;
}

static void _read_string_table(oct_file* const oct, ocl_dbuf* const buffer)
{
    size_t st_bytes = oct->header.string_table_size;
    oct->string_table = calloc(st_bytes, sizeof(oct_string));

    size_t end = _OCT_HEADER_SIZE + st_bytes;

    // Keep loading strings until we reach the table end
    while(buffer->ptr < end)
    {
        oct_string str = (oct_string){0};

        char* ptr = ocl_dbuf_pos(buffer);

        str.len = strlen(ptr);
        str.data = calloc(str.len + 1, sizeof(char));

        memcpy(str.data, ptr, str.len);
        ocl_dbuf_advance(buffer, str.len + 1);

        // Print the string entry
        //printf("%u: \"%s\"\n", oct->string_table_length, str.data);

        // Add the string to the table
        oct->string_table[oct->string_table_length++] = str;
    }

    // Shrink the string table to the amount of strings loaded
    //oct->string_table = reallocarray(oct->string_table, oct->string_table_length, sizeof(oct_string));
    oct->string_table = realloc(oct->string_table, oct->string_table_length * sizeof(oct_string));
    printf("StringTable: Loaded %u strings.\n", oct->string_table_length);
}

static void _read_data_tree(oct_file* const oct, ocl_dbuf* const buffer)
{
    size_t dt_bytes = oct->header.data_tree_size;
    oct->data_tree = calloc(dt_bytes, sizeof(oct_atomNode));

    size_t end = _OCT_HEADER_SIZE + oct->header.string_table_size + dt_bytes;

    // Keep loading atoms until we reach the end
    while(buffer->ptr < end)
    {
        oct_atomNode node = _read_atom_node(buffer, oct);
        //oct_print_atom_node(*oct, node);

        oct->data_tree[oct->data_tree_length++] = node;

        //getchar();
    }

    //oct->data_tree = reallocarray(oct->data_tree, oct->data_tree_length, sizeof(oct_atomNode));
    oct->data_tree = realloc(oct->data_tree, oct->data_tree_length * sizeof(oct_atomNode));
    printf("DataTree: Loaded %u atoms.\n", oct->data_tree_length);
}


oct_file oct_load_buffer(ocl_dbuf buffer)
{
    // Set the current target endian to machine endian
    endian_t prev_target = endian_get_target();
    endian_set_target(ENDIAN_LITTLE);

    oct_file oct = (oct_file){0};

    oct.header = _read_header(oct, &buffer);

    oct_print_header(oct.header);

    _read_string_table(&oct, &buffer);

    _read_data_tree(&oct, &buffer);

    // Reset the endian target to the original
    endian_set_target(prev_target);

    return oct;
}

oct_file oct_load_file(const char* filepath)
{
    ocl_dbuf data = ocl_dbuf_load(filepath);
    if(data.size == 0)
        return (oct_file){0};

    oct_file oct = oct_load_buffer(data);

    ocl_dbuf_free(&data);

    return oct;
}
