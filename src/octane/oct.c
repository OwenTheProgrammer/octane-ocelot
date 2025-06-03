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


void oct_file_free(oct_file* const oct)
{
    if(oct == NULL)
        return;


    _free_string_table(oct->string_table, oct->string_table_length);

    _free_data_tree(oct->data_tree, oct->data_tree_length);

}


