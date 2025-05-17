#include "oct.h"
#include "endian.h"
#include "file_io.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

static size_t _OCT_HEADER_SIZE = 60;

static const char* _OCT_UUID_KEY = "Uuid";
static const char _OCT_MAGIC_LE[8] = {0x29, 0x76, 0x01, 0x45, 0xcd, 0xcc, 0x8c, 0x3f};
static const char _OCT_MAGIC_BE[8] = {0x45, 0x01, 0x76, 0x29, 0x3f, 0x8c, 0xcc, 0xcd};

static endian_t _get_oct_endianness(oct_header header) {
    if(memcmp(header.magic, _OCT_MAGIC_LE, sizeof(header.magic)) == 0)
        return ENDIAN_LITTLE;

    if(memcmp(header.magic, _OCT_MAGIC_BE, sizeof(header.magic)) == 0)
        return ENDIAN_BIG;

    return ENDIAN_UNKNOWN;
}

static uint32_t _get_atom_array_size(io_dbuf* const buffer, unsigned int length_size)
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

static oct_nodeType _get_atom_node_type(io_dbuf* const buffer, oct_file* const oct, oct_atomNode node)
{

    int atom_type = node.header.atom_type;
    int data_type = node.header.data_type;

    oct_nodeType type = (oct_nodeType)( (int)(atom_type) | (int)(data_type<<2) );

    return type;
}

/* Parses the header section from the current pointer position */
static oct_header _oct_read_header(io_dbuf* const buffer) {
    oct_header header = (oct_header){0};

    // Read the magic section
    memcpy(header.magic, buffer->data, sizeof(header.magic));
    io_ptr_advance(buffer, sizeof(header.magic));

    // Set the global endianness
    header.file_endian = _get_oct_endianness(header);

    // We set the context endian to the files endianness
    set_endian_context(header.file_endian);

    // Ignore the padding bytes
    io_ptr_advance(buffer, 4);

    // Load the string table size (bytes)
    header.string_table_size = io_read_u32(buffer);

    // Load the data tree size (bytes)
    header.data_tree_size = io_read_u32(buffer);

    // Ignore 40 bytes of padding
    io_ptr_advance(buffer, 40);

    return header;
}

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

        str.ptr = buffer->ptr;
        str.len = strlen(str.ptr);

        // Go past the nullterm
        io_ptr_advance(buffer, str.len + 1);

        oct->string_table[oct->string_table_length++] = str;
    }

    //Resize the string table to the amount of actual strings
    oct->string_table = reallocarray(oct->string_table, oct->string_table_length, sizeof(oct_string));
    printf("StringTable: Loaded %zu strings\n", oct->string_table_length);
}

static oct_atomHeader _oct_parse_atom_header(uint16_t data)
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


static oct_atomNode _oct_read_atom_node(io_dbuf* const buffer, oct_file* const oct)
{
    oct_atomNode node = (oct_atomNode){0};

    node.header = _oct_parse_atom_header(io_read_u16(buffer));
    node.st_key = io_read_u16(buffer);

    if(node.header.has_name)
        node.name_key = io_read_u16(buffer);

    node.node_type = _get_atom_node_type(buffer, oct, node);


    uint32_t arr_size = node.header.length_size;
    uint32_t int_size = node.header.int_size;

    switch(node.node_type)
    {
        case OCT_NODE_TYPE_STRING:
            node.element_count = 1;
            node.node_data = io_read_u16_array(buffer, node.element_count);
            break;

        case OCT_NODE_TYPE_STRING_ARRAY:
            node.element_count = _get_atom_array_size(buffer, arr_size);
            node.node_data = io_read_u16_array(buffer, node.element_count);
            break;

        case OCT_NODE_TYPE_FLOAT:
            node.element_count = 1;
            node.node_data = io_read_f32_array(buffer, node.element_count);
            break;

        case OCT_NODE_TYPE_FLOAT_ARRAY:
            node.element_count = _get_atom_array_size(buffer, arr_size);
            node.node_data = io_read_f32_array(buffer, node.element_count);
            break;

        case OCT_NODE_TYPE_INT:
            node.element_count = 1;
            node.node_data = io_read_svar_array(buffer, node.element_count, int_size);
            break;

        case OCT_NODE_TYPE_INT_ARRAY:
            node.element_count = _get_atom_array_size(buffer, arr_size);
            node.node_data = io_read_svar_array(buffer, node.element_count, int_size);
            break;

        case OCT_NODE_TYPE_BINARY:
            node.element_count = _get_atom_array_size(buffer, arr_size);
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

static void _oct_read_data_tree(io_dbuf* const buffer, oct_file* const oct)
{

    oct->data_tree = calloc(oct->header.data_tree_size, sizeof(oct_atomNode));

    size_t dt_end = _OCT_HEADER_SIZE + oct->header.string_table_size + oct->header.data_tree_size;

    while(io_get_ptr_offset(buffer) < dt_end)
    {
        oct_atomNode node = _oct_read_atom_node(buffer, oct);
        oct->data_tree[oct->data_tree_length++] = node;

        //oct_print_atom_node(node, oct);

        //oct_free_atom_node(&node);
    }

    oct->data_tree = reallocarray(oct->data_tree, oct->data_tree_length, sizeof(oct_atomNode));

    printf("Data Tree: Loaded %zu atoms.\n", oct->data_tree_length);

    //oct_free_atom_node(&root_node);
}

oct_file oct_read_buffer(io_dbuf* const buffer)
{
    oct_file oct = (oct_file){0};

    // Read the header first
    oct.header = _oct_read_header(buffer);

    // Read the string table
    _oct_read_string_table(buffer, &oct);

    // Read the data tree
    _oct_read_data_tree(buffer, &oct);

    return oct;
}

oct_file oct_read_file(const char* oct_path)
{
    //Read the file data
    io_dbuf dbuf = io_load_file(oct_path);

    //Interpret the file
    oct_file oct = oct_read_buffer(&dbuf);

    //Dispose of the file buffer
    io_free_dbuf(&dbuf);

    return oct;
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

const char* _OCT_ATOM_TYPE_TABLE[OCT_ATOM_TYPE_MAX] =
{
    "Unknown",
    "Container",
    "List",
    "Single"
};
const char* _OCT_DATA_TYPE_TABLE[OCT_DATA_TYPE_MAX] =
{
    "None",
    "String",
    "Float",
    "Int",
    "Binary"
};
