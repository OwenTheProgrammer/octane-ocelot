#include "filesys/endian.h"
#include "filesys/oct.h"
#include "fs_internal.h"
#include "ocelot/memory.h"

static void _oct_write_magic_section(ocl_dbuf* const buffer)
{
    endian_t export_endian = endian_get_target();
    if(export_endian == ENDIAN_LITTLE)
    {
        ocl_dbuf_write_bytes(buffer, (char*)_OCT_MAGIC_LE, sizeof(_OCT_MAGIC_LE));
    }
    else if(export_endian == ENDIAN_BIG)
    {
        ocl_dbuf_write_bytes(buffer, (char*)_OCT_MAGIC_BE, sizeof(_OCT_MAGIC_BE));
    }
}

static ocl_dbuf _oct_encode_header_section(oct_file* const oct)
{
    ocl_dbuf buffer = ocl_dbuf_create(_OCT_HEADER_SIZE, NULL);

    _oct_write_magic_section(&buffer);

    // Skip the 4 byte padding
    ocl_dbuf_seek(&buffer, 4);

    // Write the string table size
    ocl_dbuf_write_u32(&buffer, oct->header.string_table_size);

    // Write the data tree size
    ocl_dbuf_write_u32(&buffer, oct->header.data_tree_size);

    // Seek forward 40 bytes (optional)
    ocl_dbuf_seek(&buffer, 40);

    return buffer;
}

static ocl_dbuf _oct_encode_string_table(oct_file* const oct)
{
    size_t buffer_size = oct->header.string_table_size;
    size_t buffer_count = oct->string_table_length;
    ocl_dbuf buffer = ocl_dbuf_create(buffer_size, NULL);

    for(size_t i = 0; i < buffer_count; i++)
    {
        oct_string string = oct->string_table[i];
        ocl_dbuf_write_bytes(&buffer, string.data, string.len);

        // Skip the nullterm
        buffer.ptr++;
    }

    return buffer;
}

ocl_dbuf oct_write_buffer(oct_file* const oct)
{
    // Set the context endian to the oct file
    endian_t prev_context_endian = endian_get_context();
    endian_set_context(oct->file_endian);

    ocl_dbuf buffer = _oct_encode_header_section(oct);

    ocl_dbuf string_table = _oct_encode_string_table(oct);

    ocl_dbuf combined = ocl_dbuf_merge_buffers(&buffer, &string_table, false, true);

    // Reset the endian context
    endian_set_context(prev_context_endian);
    return combined;
}
