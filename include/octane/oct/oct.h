#pragma once
#include "io/filepath.h"
#include "octane/oct/enums.h"
#include "data/endian.h"
#include "data/dbuf.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct oct_string       oct_string;
typedef struct oct_atomHeader   oct_atomHeader;
typedef struct oct_atomNode     oct_atomNode;

typedef struct oct_fileHeader   oct_fileHeader;
typedef struct oct_file         oct_file;

struct oct_string
{
    char* data;

    uint32_t len;
};

/* Implements the header section of an oct atom */
struct oct_atomHeader
{
    /* The type of this atom */
    oct_atomType atom_type;

    /* This atom has a name seperate to the type string */
    bool has_name;

    /* The type of each element for list or element atoms */
    oct_dataType data_type;

    /* The stride of a list atoms element count value in bytes */
    uint32_t length_bytes;

    /* The stride of each element in bytes */
    uint32_t element_bytes;

    /* The hierarchy depth of this atom, root being 0 */
    uint32_t tree_level;
};

/* Implements an atom from the oct file format */
struct oct_atomNode
{
    /* The header section of the atom */
    oct_atomHeader header;

    /* String table index representing this atoms type name */
    uint16_t st_key;

    /* String table index for this atoms name if `has_name` is true */
    uint16_t name_key;

    /* Amount of elements this atom stores */
    uint32_t elem_count;

    /* The raw data stored in this atom */
    void* data;
};

/* */
struct oct_fileHeader
{
    endian_t endian;

    // char magic[4];

    float version;

    // uint32_t cache_crc;

    uint32_t string_table_size;

    uint32_t data_tree_size;

    // char pad[40];
};

struct oct_file
{
    filepath_t world_path;

    oct_fileHeader header;

    uint32_t string_table_length;

    oct_string* string_table;


    uint32_t data_tree_length;

    oct_atomNode* data_tree;
};

ocl_nodeType ocl_parse_atom_node_type(oct_atomHeader header);

oct_file oct_load_buffer(dbuf buffer);

oct_file oct_load_file(const char* world_path);


dbuf oct_store_buffer(oct_file oct);

void oct_store_file(oct_file oct, const char* filepath);


void oct_file_free(oct_file* const oct);
