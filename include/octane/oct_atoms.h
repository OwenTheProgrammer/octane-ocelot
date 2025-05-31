#pragma once
#include "octane/oct.h"
#include <stdint.h>

typedef enum
{
    OCT_PRIMITIVE_TYPE_NONE,
    OCT_PRIMITIVE_TYPE_TRIANGLES,
    OCT_PRIMITIVE_TYPE_MAX
} oct_primitiveType;

typedef struct oct_atomNameTable    oct_atomNameTable;
typedef struct oct_indexStreamAtom  oct_indexStreamAtom;

struct oct_atomNameTable
{
    uint16_t length;

    uint16_t index_stream_pool;

    uint16_t index_stream;

    uint16_t index_buffer_reference;

    uint16_t index_buffer_offset;

    uint16_t index_stream_primitive;
};

struct oct_indexStreamAtom
{
    uint16_t name_key;

    uint32_t length;

    uint32_t buffer_reference;

    uint32_t buffer_offset;

    oct_primitiveType stream_primitive;
};


/* Returns the string table index of a string, 0 if not found */
uint16_t oct_get_st_index_of(oct_file oct, const char* str);

oct_atomNameTable oct_create_atom_name_table(oct_file oct);

void oct_print_atom_name_table(oct_file oct, oct_atomNameTable ant);
