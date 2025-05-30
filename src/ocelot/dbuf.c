#include "ocelot/dbuf.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bool _is_valid_dbuf(ocl_dbuf* const buffer)
{
    if(buffer == NULL)
        return false;

    return (buffer->data != NULL && buffer->size != 0);
}

ocl_dbuf ocl_dbuf_create(size_t size, void* const data)
{
    ocl_dbuf result = (ocl_dbuf){0};

    if(size == 0)
        return result;

    result.size = size;
    result.data = calloc(size, sizeof(char));

    if(data != NULL)
    {
        memcpy(result.data, data, size);
        result.ptr = size;
    }

    return result;
}

ocl_dbuf ocl_dbuf_load(const char* filepath)
{
    ocl_dbuf buffer = (ocl_dbuf){0};

    if(filepath == NULL)
    {
        fprintf(stderr, "Invalid filepath.\n");
        return buffer;
    }

    FILE* file = fopen(filepath, "rb");

    if(file == NULL)
    {
        fprintf(stderr, "Failed to read file \"%s\".\n", filepath);
        return buffer;
    }

    // Capture the file size
    fseek(file, 0, SEEK_END);
    buffer.size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate and read the file data
    buffer.data = calloc(buffer.size, sizeof(char));
    fread(buffer.data, sizeof(char), buffer.size, file);

    // Close the file handle
    fclose(file);

    printf("Loaded \"%s\" (%zu bytes)\n", filepath, buffer.size);

    return buffer;
}

void ocl_dbuf_write(ocl_dbuf buffer, const char* filepath)
{
    if(buffer.data == NULL || buffer.size == 0)
    {
        fprintf(stderr, "Invalid data buffer.\n");
        return;
    }

    if(filepath == NULL)
    {
        fprintf(stderr, "Invalid filepath.\n");
        return;
    }

    FILE* file = fopen(filepath, "wb");
    if(file == NULL)
    {
        fprintf(stderr, "Failed to write file \"%s\"\n", filepath);
        return;
    }

    fwrite(buffer.data, sizeof(char), buffer.size, file);
    fflush(file);
    fclose(file);

    printf("Write %zu bytes to \"%s\" successfully.\n", buffer.size, filepath);
}


void ocl_dbuf_ensure_capacity(ocl_dbuf* const buffer, size_t bytes)
{
    if(buffer == NULL || buffer->data == NULL)
    {
        *buffer = ocl_dbuf_create(bytes, NULL);
        return;
    }

    size_t region_size = buffer->ptr + bytes;
    if(region_size > buffer->size)
    {
        buffer->data = reallocarray(buffer->data, region_size, sizeof(char));
        buffer->size = region_size;
    }
}

void ocl_dbuf_advance(ocl_dbuf* const buffer, size_t bytes)
{
    buffer->ptr += bytes;
}

void* ocl_dbuf_pos(ocl_dbuf* const buffer)
{
    return buffer->data + buffer->ptr;
}


void ocl_dbuf_write_data(ocl_dbuf* const buffer, void* const src, size_t bytes, bool advance)
{
    if(bytes == 0)
        return;

    void* dst_ptr = ocl_dbuf_pos(buffer);

    ocl_dbuf_ensure_capacity(buffer, bytes);

    if(src != NULL)
    {
        memcpy(dst_ptr, src, bytes);
    }
    else
    {
        memset(dst_ptr, 0, bytes);
    }

    if(advance)
        ocl_dbuf_advance(buffer, bytes);
}

ocl_dbuf ocl_dbuf_merge(bool end_at_ptr, bool free_inputs, size_t count, ...)
{
    // Make a reference pool of all the valid buffers
    ocl_dbuf** pool = calloc(count, sizeof(ocl_dbuf*));

    // Keep track of the final buffer metrics
    size_t valid_count = 0;
    size_t final_size = 0;

    va_list args;
    va_start(args, count);

    for(size_t i = 0; i < count; i++)
    {
        ocl_dbuf* entry = va_arg(args, ocl_dbuf*);

        if(_is_valid_dbuf(entry))
        {
            pool[valid_count++] = entry;
            final_size += end_at_ptr ? entry->ptr : entry->size;
        }
    }

    va_end(args);

    // Combine all the buffers

    ocl_dbuf result = (ocl_dbuf){0};

    result.size = final_size;
    result.data = calloc(final_size, sizeof(char));

    for(size_t i = 0; i < valid_count; i++)
    {
        ocl_dbuf* entry = pool[i];

        size_t len = end_at_ptr ? entry->ptr : entry->size;
        memcpy(ocl_dbuf_pos(&result), entry->data, len);

        ocl_dbuf_advance(&result, len);

        if(free_inputs)
            ocl_dbuf_free(entry);
    }

    free(pool);
    return result;
}



void ocl_dbuf_free(ocl_dbuf* const buffer)
{
    if(buffer != NULL)
    {
        free(buffer->data);

        *buffer = (ocl_dbuf){0};
    }
}
