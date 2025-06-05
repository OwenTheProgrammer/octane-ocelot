#include "octane/oct.h"
#include "ocelot/endian.h"
#include "oct_internal.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

const size_t _OCT_MAGIC_SIZE = 4;
const size_t _OCT_HEADER_SIZE = 60;

const uint32_t _OCT_MAGIC_LE = 0x45017629;
const uint32_t _OCT_MAGIC_BE = 0x29760145;

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


endian_t _oct_magic_to_endian(uint32_t* const magic)
{
    if(*magic == _OCT_MAGIC_LE)
        return ENDIAN_LITTLE;

    if(*magic == _OCT_MAGIC_BE)
        return ENDIAN_BIG;

    return ENDIAN_UNKNOWN;
}

uint32_t _oct_endian_to_magic(endian_t endian)
{
    return (endian == ENDIAN_LITTLE) ? _OCT_MAGIC_LE : _OCT_MAGIC_BE;
}


void oct_file_free(oct_file* const oct)
{
    if(oct == NULL)
        return;


    _free_string_table(oct->string_table, oct->string_table_length);

    _free_data_tree(oct->data_tree, oct->data_tree_length);

}


