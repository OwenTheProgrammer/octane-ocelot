#include "common/endian.h"
#include "oct_internal.h"
#include "octane/oct.h"
#include "common/dbuf.h"
#include "utils.h"
#include <stdio.h>
#include <stdbool.h>

#define STORE_VALUE(VALUE, LEN)                             \
    {                                                       \
        result |= NMASK((uint16_t)(VALUE), LEN) << shift;   \
        shift += (LEN);                                     \
    }

static uint16_t _encode_atom_header(oct_atomHeader header)
{
    uint16_t result = 0;
    unsigned int shift = 0;

    STORE_VALUE(header.atom_type, 2);
    STORE_VALUE(header.has_name, 1);
    STORE_VALUE(header.data_type, 3);
    STORE_VALUE(header.length_bytes-1, 2);
    STORE_VALUE(header.element_bytes-1, 2);
    STORE_VALUE(header.tree_level, 6);

    return result;
}

#undef STORE_VALUE

static void _encode_atom_node(dbuf* const buffer, oct_atomNode node)
{
    // Write the atom header segment and match the current endian
    uint16_t header = _encode_atom_header(node.header);
    header = endian_eval_u16(header, ENDIAN_LITTLE);

    dbuf_write_u16(buffer, header);

    // Write the string table key
    dbuf_write_u16(buffer, node.st_key);

    // Write the name key if its present
    if(node.header.has_name)
        dbuf_write_u16(buffer, node.name_key);

    uint32_t list_stride = node.header.length_bytes;
    uint32_t elem_stride = node.header.element_bytes;

    switch(node.node_type)
    {
        case OCT_NODE_TYPE_STRING:
            dbuf_write_u16(buffer, *(uint16_t*)node.data);
            break;

        case OCT_NODE_TYPE_STRING_ARRAY:
            dbuf_write_uvar(buffer, node.elem_count, list_stride);
            dbuf_write_u16_array(buffer, (uint16_t*)node.data, node.elem_count);
            break;

        case OCT_NODE_TYPE_FLOAT:
            dbuf_write_u32(buffer, *(uint32_t*)node.data);
            break;

        case OCT_NODE_TYPE_FLOAT_ARRAY:
            dbuf_write_uvar(buffer, node.elem_count, list_stride);
            dbuf_write_u32_array(buffer, (uint32_t*)node.data, node.elem_count);
            break;

        case OCT_NODE_TYPE_INT:
            dbuf_write_uvar(buffer, *(uint32_t*)node.data, elem_stride);
            break;

        case OCT_NODE_TYPE_INT_ARRAY:
            dbuf_write_uvar(buffer, node.elem_count, list_stride);
            dbuf_write_uvar_array(buffer, (uint32_t*)node.data, node.elem_count, elem_stride);
            break;

        case OCT_NODE_TYPE_BINARY:
        case OCT_NODE_TYPE_UUID:
            dbuf_write_uvar(buffer, node.elem_count, list_stride);
            dbuf_write_u8_array(buffer, (uint8_t*)node.data, node.elem_count);
            break;

        default:
            break;
    }
}

static dbuf _encode_header(oct_header header)
{
    dbuf buffer = dbuf_create(_OCT_HEADER_SIZE, NULL);

    // Write the magic section (endian swapping will convert LE to BE if needed)
    //dbuf_write_u32(&buffer, _oct_endian_to_magic(endian_get_target()));
    dbuf_write_u32(&buffer, _OCT_MAGIC_LE);

    // Write the header version
    dbuf_write_u32(&buffer, *(uint32_t*)&header.version);

    // Skip the cache crc
    dbuf_advance(&buffer, 4);

    // Write the string table size (bytes)
    dbuf_write_u32(&buffer, header.string_table_size);

    // Write the data tree size (bytes)
    dbuf_write_u32(&buffer, header.data_tree_size);

    // Ignore the next 40 bytes
    // dbuf_advance(&buffer, 40);

    return buffer;
}

static dbuf _encode_string_table(oct_file oct)
{
    dbuf buffer = dbuf_create(oct.header.string_table_size, NULL);

    for(uint32_t i = 0; i < oct.string_table_length; i++)
    {
        oct_string str = oct.string_table[i];

        dbuf_write_data(&buffer, str.data, str.len, false);
        dbuf_advance(&buffer, str.len + 1);
    }

    return buffer;
}

static dbuf _encode_data_tree(oct_file oct)
{
    dbuf buffer = dbuf_create(oct.header.data_tree_size, NULL);

    for(uint32_t i = 0; i < oct.data_tree_length; i++)
    {
        oct_atomNode node = oct.data_tree[i];
        //oct_print_atom_node(oct, node);

        //size_t _from = buffer.ptr;
        _encode_atom_node(&buffer, node);

        //size_t _bytes = buffer.ptr - _from;
        //printf("Encoded %zu bytes\n", _bytes);
        //phex_u8_array(buffer.data + _from, _bytes);

        //getchar();
    }

    return buffer;
}

dbuf oct_store_buffer(oct_file oct)
{
    endian_t prev_ctx = endian_get_current();
    endian_set_current(oct.header.endian);

    printf("Writing oct file V%.2f (Endian: %s -> %s)\n",
           oct.header.version,
           _ENDIAN_PRINT_TABLE[(int)endian_get_current()],
           _ENDIAN_PRINT_TABLE[(int)endian_get_target()]
    );

    dbuf header = _encode_header(oct.header);

    dbuf string_table = _encode_string_table(oct);

    dbuf data_tree = _encode_data_tree(oct);

    dbuf buffer = dbuf_merge(false, true, 3, &header, &string_table, &data_tree);

    endian_set_current(prev_ctx);
    return buffer;
}

void oct_store_file(oct_file oct, const char* filepath)
{
    dbuf data = oct_store_buffer(oct);

    dbuf_write(data, filepath);

    dbuf_free(&data);
}
