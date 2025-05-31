/* oct: The octane scene descriptor */
#pragma once
#include "utils.h"
#include "byteconv/endian.h"
#include "ocelot/dbuf.h"
#include <stdint.h>


typedef enum
{
    OCT_ATOM_TYPE_UNKNOWN   = 0b00,
    OCT_ATOM_TYPE_CONTAINER = 0b01,
    OCT_ATOM_TYPE_LIST      = 0b10,
    OCT_ATOM_TYPE_ELEMENT   = 0b11,
    OCT_ATOM_TYPE_MAX
} oct_atomType;

typedef enum
{
    OCT_DATA_TYPE_NONE      = 0b000,
    OCT_DATA_TYPE_STRING    = 0b001,
    OCT_DATA_TYPE_FLOAT     = 0b010,
    OCT_DATA_TYPE_INT       = 0b011,
    OCT_DATA_TYPE_BINARY    = 0b100,
    OCT_DATA_TYPE_MAX
} oct_dataType;

#define NODE_TYPE(AT, DT) ( (OCT_ATOM_TYPE_##AT) | ((OCT_DATA_TYPE_##DT)<<2) )

typedef enum
{
    OCT_NODE_TYPE_BRANCH        = NODE_TYPE(CONTAINER, NONE),
    OCT_NODE_TYPE_STRING        = NODE_TYPE(ELEMENT, STRING),
    OCT_NODE_TYPE_STRING_ARRAY  = NODE_TYPE(LIST, STRING),
    OCT_NODE_TYPE_FLOAT         = NODE_TYPE(ELEMENT, FLOAT),
    OCT_NODE_TYPE_FLOAT_ARRAY   = NODE_TYPE(LIST, FLOAT),
    OCT_NODE_TYPE_INT           = NODE_TYPE(ELEMENT, INT),
    OCT_NODE_TYPE_INT_ARRAY     = NODE_TYPE(LIST, INT),
    OCT_NODE_TYPE_BINARY        = NODE_TYPE(ELEMENT, BINARY),
    OCT_NODE_TYPE_UUID          = NODE_TYPE(LIST, BINARY),
    OCT_NODE_TYPE_MAX           = 9
} oct_nodeType;

#undef NODE_TYPE


extern const char* _OCT_ATOM_TYPE_PRINT_TABLE[OCT_ATOM_TYPE_MAX];
extern const char* _OCT_DATA_TYPE_PRINT_TABLE[OCT_DATA_TYPE_MAX];

typedef struct oct_string       oct_string;
typedef struct oct_atomHeader   oct_atomHeader;
typedef struct oct_atomNode     oct_atomNode;

typedef struct oct_header       oct_header;
typedef struct oct_file         oct_file;

struct oct_string
{
    char* data;

    uint32_t len;
};

struct oct_atomHeader
{
    oct_atomType atom_type;

    bool_t has_name;

    oct_dataType data_type;

    uint32_t length_bytes;

    uint32_t element_bytes;

    uint32_t tree_level;
};

struct oct_atomNode
{
    oct_atomHeader header;

    uint16_t st_key;

    uint16_t name_key;

    oct_nodeType node_type;

    uint32_t elem_count;

    void* data;
};

struct oct_header
{
    endian_t endian;

    // char magic[8];

    // char pad[4];

    uint32_t string_table_size;

    uint32_t data_tree_size;

    // char pad[40];
};

struct oct_file
{
    oct_header header;

    uint32_t string_table_length;

    oct_string* string_table;


    uint32_t data_tree_length;

    oct_atomNode* data_tree;
};

oct_file oct_load_buffer(ocl_dbuf buffer);

oct_file oct_load_file(const char* filepath);


ocl_dbuf oct_store_buffer(oct_file oct);

void oct_store_file(oct_file oct, const char* filepath);



void oct_print_header(oct_header header);
void oct_print_atom_header(oct_atomHeader header);
void oct_print_atom_node(oct_file oct, oct_atomNode node);


void oct_file_free(oct_file* const oct);
