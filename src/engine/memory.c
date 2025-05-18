#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ocelot/memory.h"
#include "filesys/endian.h"

ocl_dbuf ocl_dbuf_create(size_t size, void* data)
{
    ocl_dbuf buffer = (ocl_dbuf){0};

    buffer.size = size;

    if(size != 0)
        buffer.memory = calloc(size, sizeof(char));

    if(data != NULL && size != 0 && buffer.memory != NULL)
    {
        memcpy(buffer.memory, data, size);
    }

    buffer.ptr = buffer.memory;

    return buffer;
}

void ocl_dbuf_export(ocl_dbuf* const buffer, const char* filepath)
{
    if(ocl_dbuf_is_valid(buffer) && filepath != NULL)
    {
        FILE* file = fopen(filepath, "wb");
        if(file == NULL)
        {
            fprintf(stderr, "Failed to open file \"%s\"\n", filepath);
            return;
        }

        fwrite(buffer->memory, sizeof(char), buffer->size, file);
        fflush(file);
        fclose(file);

        printf("Exported %zu bytes to \"%s\".\n", buffer->size, filepath);
    }
    else
    {
        printf("Export halted: nothing to write.\n");
    }
}


bool ocl_dbuf_is_valid(ocl_dbuf* const buffer)
{
    if(buffer == NULL)
        return false;

    return (buffer->memory != NULL) && (buffer->size != 0);
}

void ocl_dbuf_seek(ocl_dbuf* const buffer, int bytes)
{
    if(ocl_dbuf_is_valid(buffer))
        buffer->ptr += bytes;
}

size_t ocl_dbuf_get_ptr_offset(ocl_dbuf* const buffer)
{
    if(ocl_dbuf_is_valid(buffer))
        return (size_t)(buffer->ptr - buffer->memory);

    return 0;
}


void ocl_dbuf_writens_bytes(ocl_dbuf* const buffer, void* const src, size_t src_size)
{
    //Dont need to write anything
    if(src_size == 0)
        return;

    if(buffer == NULL)
    {
        fprintf(stderr, "Data buffer can not be null.\n");
        return;
    }

    size_t ptr_offset = ocl_dbuf_get_ptr_offset(buffer);
    size_t region_end = ptr_offset + src_size;

    //We need more space
    if(region_end >= buffer->size)
    {
        buffer->memory = reallocarray(buffer->memory, region_end, sizeof(char));
        buffer->size = region_end;
    }

    if(src == NULL)
    {
        memset(buffer->ptr, 0, src_size);
    }
    else
    {
        memcpy(buffer->ptr, src, src_size);
    }
}

void ocl_dbuf_write_bytes(ocl_dbuf* const buffer, void* const src, size_t src_size)
{
    ocl_dbuf_writens_bytes(buffer, src, src_size);
    ocl_dbuf_seek(buffer, src_size);
}


void ocl_dbuf_write_u32(ocl_dbuf* const buffer, uint32_t value)
{
    //Swap endian if needed
    uint32_t data = endian_swap_u32(value);
    ocl_dbuf_write_bytes(buffer, &data, sizeof(uint32_t));
}

void ocl_dbuf_write_u32_array(ocl_dbuf* const buffer, uint32_t* const data, size_t count)
{
    // TODO: do validation on the buffers first for optimization

    //Create a new array and swap endian of each element if needed
    uint32_t* memory = endian_swap_u32_array(data, count);

    // Write the contents to the data buffer
    ocl_dbuf_write_bytes(buffer, memory, count * sizeof(uint32_t));

    // Dispose of the temporary buffer data
    free(memory);
}


void ocl_dbuf_free(ocl_dbuf* const buffer)
{
    if(buffer != NULL && buffer->memory != NULL)
    {
        free(buffer->memory);
        *buffer = (ocl_dbuf){0};
    }
}
