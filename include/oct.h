#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "endian.h"
#include "file_io.h"

//Type
typedef enum
{
    OCT_ATOM_TYPE_UNKNOWN   = 0b00,
    OCT_ATOM_TYPE_CONTAINER = 0b01,
    OCT_ATOM_TYPE_LIST      = 0b10,
    OCT_ATOM_TYPE_SINGLE    = 0b11,
    OCT_ATOM_TYPE_MAX
} oct_atomType;

extern const char* _OCT_ATOM_TYPE_TABLE[OCT_ATOM_TYPE_MAX];

//DataType
typedef enum
{
    OCT_DATA_TYPE_NONE      = 0b000,
    OCT_DATA_TYPE_STRING    = 0b001,
    OCT_DATA_TYPE_FLOAT     = 0b010,
    OCT_DATA_TYPE_INT       = 0b011,
    OCT_DATA_TYPE_BINARY    = 0b100,
    OCT_DATA_TYPE_MAX
} oct_dataType;

extern const char* _OCT_DATA_TYPE_TABLE[OCT_DATA_TYPE_MAX];

#define NODE_TYPE(AT, DT) ( (OCT_ATOM_TYPE_##AT) | ((OCT_DATA_TYPE_##DT)<<2) )

typedef enum
{
    OCT_NODE_TYPE_BRANCH        = NODE_TYPE(CONTAINER, NONE),
    OCT_NODE_TYPE_STRING        = NODE_TYPE(SINGLE, STRING),
    OCT_NODE_TYPE_STRING_ARRAY  = NODE_TYPE(LIST, STRING),
    OCT_NODE_TYPE_FLOAT         = NODE_TYPE(SINGLE, FLOAT),
    OCT_NODE_TYPE_FLOAT_ARRAY   = NODE_TYPE(LIST, FLOAT),
    OCT_NODE_TYPE_INT           = NODE_TYPE(SINGLE, INT),
    OCT_NODE_TYPE_INT_ARRAY     = NODE_TYPE(LIST, INT),
    OCT_NODE_TYPE_UUID          = NODE_TYPE(UNKNOWN, BINARY),
    OCT_NODE_TYPE_BINARY        = NODE_TYPE(SINGLE, BINARY),
    OCT_NODE_TYPE_MAX
} oct_nodeType;

#undef NODE_TYPE

typedef struct oct_header       oct_header;
typedef struct oct_string       oct_string;

typedef struct oct_atomHeader   oct_atomHeader;
typedef struct oct_atomNode     oct_atomNode;

typedef struct oct_file     oct_file;

struct oct_header
{
    /* File magic ID */
    uint8_t magic[8];

    /* 4 byte padding */
    //char _pad[4];

    /* Size of the string table in bytes */
    uint32_t string_table_size;

    /* Size of the data tree in bytes */
    uint32_t data_tree_size;

    /* 40 byte padding */
    //char _pad[40];

    // == NOT STORED == //

    /* The endianness of the file */
    endian_t file_endian;
};

struct oct_string
{
    /* Pointer to the string location */
    char* ptr;

    /* Length of the string */
    size_t len;
};

struct oct_atomHeader
{
    oct_atomType atom_type;

    bool has_name;

    oct_dataType data_type;

    /* Amount of bytes after this atom header used to store 
     * the length of a list or scalar atoms elements.
     * 00 = 1 byte .. 11 = 4 bytes
     */
    unsigned int length_size;

    /* 00 = 1 byte .. 11 = 4 bytes */
    unsigned int int_size;

    /* The hierarchy level of this atom [6b] */
    unsigned int level;
};

struct oct_atomNode
{
    oct_atomHeader header;

    uint16_t st_key;

    uint16_t name_key;

    oct_nodeType node_type;

    /* Amount of elements stored in node_data */
    size_t element_count;

    void* node_data;
};


struct oct_file
{
    oct_header header;

    /* Amount of elements in the string table */
    size_t string_table_length;

    /* The string table */
    oct_string* string_table;

    /* Amount of elements in the data tree */
    size_t data_tree_length;

    /* The tree of atoms */
    oct_atomNode* data_tree;
};

/* Reads an oct file from a data buffer */
oct_file oct_read_buffer(io_dbuf* const buffer);

/* Reads an oct file from a given filepath */
oct_file oct_read_file(const char* oct_path);

/* Dispose of an oct files allocated memory */
void oct_free_file(oct_file* const oct);

void oct_free_atom_node(oct_atomNode* const node);

void oct_print_header(oct_header header);
void oct_print_atom_header(oct_atomHeader header);
void oct_print_atom_node(oct_atomNode node, oct_file* const oct);
