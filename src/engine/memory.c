#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "ocelot/memory.h"
#include "filesys/endian.h"

ocl_dbuf ocl_dbuf_create(size_t size, void* data)
{
    ocl_dbuf buffer = (ocl_dbuf){0};

    buffer.size = size;

    if(size != 0)
    {
        buffer.memory = calloc(size, sizeof(char));
        buffer.ptr = buffer.memory;
    }

    if(data != NULL && size != 0 && buffer.memory != NULL)
    {
        memcpy(buffer.memory, data, size);
        buffer.ptr += size;
    }

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


void ocl_dbuf_shrink_to_fit(ocl_dbuf* const buffer)
{
    if(buffer != NULL)
        buffer->size = ocl_dbuf_get_ptr_offset(buffer);
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
        //reallocarray moves the memory address of the buffer
        buffer->ptr = buffer->memory + ptr_offset;

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

ocl_dbuf ocl_dbuf_fullcopy_buffer(ocl_dbuf* const buffer)
{
    ocl_dbuf result = (ocl_dbuf){0};
    if(!ocl_dbuf_is_valid(buffer))
        return result;

    result = ocl_dbuf_create(buffer->size, buffer->memory);
    return result;
}


ocl_dbuf ocl_dbuf_merge_buffers(bool end_at_ptr, bool free_inputs, size_t count, ...)
{
    // Make a reference pool for all the valid buffers
    ocl_dbuf** pool = calloc(count, sizeof(ocl_dbuf*));

    // Keep track of the total size
    size_t valid_count = 0;
    size_t final_size = 0;

    va_list args;
    va_start(args, count);

    for(size_t i = 0; i < count; i++)
    {
        ocl_dbuf* entry = va_arg(args, ocl_dbuf*);

        if(ocl_dbuf_is_valid(entry))
        {
            pool[valid_count++] = entry;
            final_size += end_at_ptr ? ocl_dbuf_get_ptr_offset(entry) : entry->size;
        }
    }

    va_end(args);

    // Combine all the buffers together
    ocl_dbuf result = (ocl_dbuf){0};
    result.size = final_size;
    result.memory = calloc(final_size, sizeof(char));
    result.ptr = result.memory;

    for(size_t i = 0; i < count; i++)
    {
        size_t len = end_at_ptr ? ocl_dbuf_get_ptr_offset(pool[i]) : pool[i]->size;
        memcpy(result.ptr, pool[i]->memory, len);
        result.ptr += len;

        if(free_inputs)
            ocl_dbuf_free(pool[i]);
    }

    free(pool);
    return result;
}


void ocl_dbuf_free(ocl_dbuf* const buffer)
{
    if(buffer != NULL && buffer->memory != NULL)
    {
        free(buffer->memory);
        *buffer = (ocl_dbuf){0};
    }
}
