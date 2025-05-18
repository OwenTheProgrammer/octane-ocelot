#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <malloc.h>
#include <string.h>

#include "filesys/file_io.h"
#include "filesys/endian.h"
#include "filesys/oct.h"
#include "fs_internal.h"

/* Reads the length of an oct atoms array */
static uint32_t _oct_get_atom_array_size(io_dbuf* const buffer, unsigned int length_size)
{
    if(length_size > 4)
    {
        fprintf(stderr, "Atom array length can not be greater than 4. length: %u\n", length_size);
        return UINT32_MAX;
    }

    // Read the next length_size bytes (without seeking)
    uint32_t length = io_readns_uvar(buffer, length_size);

    // Seek forward
    io_ptr_advance(buffer, length_size);

    return length;
}

/* Parses the header section from the current pointer position */
static void _oct_read_header(io_dbuf* const buffer, oct_file* const oct) {
    oct_header header = (oct_header){0};

    // Read the input buffers magic section
    endian_t buffer_magic = _oct_get_magic_endian(buffer->ptr);

    if(buffer_magic == ENDIAN_UNKNOWN)
    {
        fprintf(stderr, "Unknown magic!\n");
        oct->header = header;
        oct->file_endian = ENDIAN_UNKNOWN;
        return;
    }

    // Seek forward past the magic section
    io_ptr_advance(buffer, 8);

    // Set the endian context to the buffers endian
    endian_set_context(buffer_magic);

    // Set the new endian to the target endian
    oct->file_endian = endian_get_target();

    printf("Reading oct file (endian: %s -> %s)\n", 
           _ENDIAN_PRINT_TABLE[(int)buffer_magic], 
           _ENDIAN_PRINT_TABLE[(int)oct->file_endian]
    );

    // Ignore the padding bytes
    io_ptr_advance(buffer, 4);

    // Load the string table size (bytes)
    header.string_table_size = io_read_u32(buffer);

    // Load the data tree size (bytes)
    header.data_tree_size = io_read_u32(buffer);

    // Ignore 40 bytes of padding
    io_ptr_advance(buffer, 40);

    oct->header = header;
}

/* Reads an oct_atomNode from an oct data buffer */
static oct_atomNode _oct_read_atom_node(io_dbuf* const buffer, oct_file* const oct)
{
    oct_atomNode node = (oct_atomNode){0};

    node.header = _oct_parse_atom_header(io_read_u16(buffer));
    node.st_key = io_read_u16(buffer);

    if(node.header.has_name)
        node.name_key = io_read_u16(buffer);

    node.node_type = _oct_get_atom_node_type(buffer, node);


    uint32_t arr_size = node.header.length_size;
    uint32_t int_size = node.header.int_size;

    switch(node.node_type)
    {
        case OCT_NODE_TYPE_STRING:
            node.element_count = 1;
            node.node_data = io_read_u16_array(buffer, node.element_count);
            break;

        case OCT_NODE_TYPE_STRING_ARRAY:
            node.element_count = _oct_get_atom_array_size(buffer, arr_size);
            node.node_data = io_read_u16_array(buffer, node.element_count);
            break;

        case OCT_NODE_TYPE_FLOAT:
            node.element_count = 1;
            node.node_data = io_read_f32_array(buffer, node.element_count);
            break;

        case OCT_NODE_TYPE_FLOAT_ARRAY:
            node.element_count = _oct_get_atom_array_size(buffer, arr_size);
            node.node_data = io_read_f32_array(buffer, node.element_count);
            break;

        case OCT_NODE_TYPE_INT:
            node.element_count = 1;
            node.node_data = io_read_svar_array(buffer, node.element_count, int_size);
            break;

        case OCT_NODE_TYPE_INT_ARRAY:
            node.element_count = _oct_get_atom_array_size(buffer, arr_size);
            node.node_data = io_read_svar_array(buffer, node.element_count, int_size);
            break;

        case OCT_NODE_TYPE_BINARY:
            node.element_count = _oct_get_atom_array_size(buffer, arr_size);
            node.node_data = io_read_u8_array(buffer, node.element_count);
            break;

        case OCT_NODE_TYPE_UUID:
            node.element_count = 16;
            // TODO: actually decode the UUID as its supposed to be formated
            node.node_data = io_read_u8_array(buffer, node.element_count);
            break;

        default:
            break;
    }

    return node;
}

/* Reads the string table section of an oct file */
static void _oct_read_string_table(io_dbuf* const buffer, oct_file* const oct)
{
    // Allocate space in the string table, more than we need
    // since we dont know how many strings there are.
    oct->string_table = calloc(oct->header.string_table_size, sizeof(oct_string));

    // The position we stop reading strings at
    size_t table_end = _OCT_HEADER_SIZE + oct->header.string_table_size;

    while(io_get_ptr_offset(buffer) < table_end)
    {
        oct_string str = (oct_string){0};

        str.len = strlen(buffer->ptr);

        str.data = calloc(str.len + 1, sizeof(char));
        memcpy(str.data, buffer->ptr, str.len);

        // Go past the nullterm
        io_ptr_advance(buffer, str.len + 1);

        oct->string_table[oct->string_table_length++] = str;
    }

    //Resize the string table to the amount of actual strings
    oct->string_table = reallocarray(oct->string_table, oct->string_table_length, sizeof(oct_string));
    printf("StringTable: Loaded %zu strings\n", oct->string_table_length);
}

/* Reads the data tree section of an oct file */
static void _oct_read_data_tree(io_dbuf* const buffer, oct_file* const oct)
{

    oct->data_tree = calloc(oct->header.data_tree_size, sizeof(oct_atomNode));

    size_t dt_end = _OCT_HEADER_SIZE + oct->header.string_table_size + oct->header.data_tree_size;

    while(io_get_ptr_offset(buffer) < dt_end)
    {
        oct_atomNode node = _oct_read_atom_node(buffer, oct);
        oct->data_tree[oct->data_tree_length++] = node;
    }

    oct->data_tree = reallocarray(oct->data_tree, oct->data_tree_length, sizeof(oct_atomNode));
    printf("Data Tree: Loaded %zu atoms.\n", oct->data_tree_length);
}

oct_file oct_read_buffer(io_dbuf* const buffer)
{
    oct_file oct = (oct_file){0};

    // Read the header first
    _oct_read_header(buffer, &oct);

    // Read the string table
    _oct_read_string_table(buffer, &oct);

    // Read the data tree
    _oct_read_data_tree(buffer, &oct);

    return oct;
}


oct_file oct_read_file(const char* oct_path, endian_t target_endian)
{
    //Switch the current target endian
    endian_t prev_endian = endian_get_target();
    endian_set_target(target_endian);

    //Read the file data
    io_dbuf dbuf = io_load_file(oct_path);

    //Interpret the file
    oct_file oct = oct_read_buffer(&dbuf);

    //Dispose of the file buffer
    io_free_dbuf(&dbuf);

    //Revert back to the original endian
    endian_set_target(prev_endian);
    return oct;
}
