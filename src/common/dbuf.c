#include "common/dbuf.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


dbuf dbuf_create(size_t size, void* const data)
{
    dbuf result = (dbuf){0};

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

dbuf dbuf_load(const char* filepath)
{
    dbuf buffer = (dbuf){0};

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

bool dbuf_is_valid(dbuf* const buffer)
{
    if(buffer == NULL)
        return false;

    return (buffer->data != NULL && buffer->size != 0);
}

void dbuf_write(dbuf buffer, const char* filepath)
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


void dbuf_ensure_capacity(dbuf* const buffer, size_t bytes)
{
    if(buffer == NULL || buffer->data == NULL)
    {
        *buffer = dbuf_create(bytes, NULL);
        return;
    }

    size_t region_size = buffer->ptr + bytes;
    if(region_size > buffer->size)
    {
        //buffer->data = reallocarray(buffer->data, region_size, sizeof(char));
        buffer->data = realloc(buffer->data, region_size * sizeof(char));
        buffer->size = region_size;
    }
}

void dbuf_advance(dbuf* const buffer, size_t bytes)
{
    buffer->ptr += bytes;
}

void* dbuf_pos(dbuf* const buffer)
{
    return buffer->data + buffer->ptr;
}


void dbuf_write_data(dbuf* const buffer, void* const src, size_t bytes, bool advance)
{
    if(bytes == 0)
        return;

    void* dst_ptr = dbuf_pos(buffer);

    dbuf_ensure_capacity(buffer, bytes);

    if(src != NULL)
    {
        memcpy(dst_ptr, src, bytes);
    }
    else
    {
        memset(dst_ptr, 0, bytes);
    }

    if(advance)
        dbuf_advance(buffer, bytes);
}

dbuf dbuf_merge(bool end_at_ptr, bool free_inputs, size_t count, ...)
{
    // Make a reference pool of all the valid buffers
    dbuf** pool = calloc(count, sizeof(dbuf*));

    // Keep track of the final buffer metrics
    size_t valid_count = 0;
    size_t final_size = 0;

    va_list args;
    va_start(args, count);

    for(size_t i = 0; i < count; i++)
    {
        dbuf* entry = va_arg(args, dbuf*);

        if(dbuf_is_valid(entry))
        {
            pool[valid_count++] = entry;
            final_size += end_at_ptr ? entry->ptr : entry->size;
        }
    }

    va_end(args);

    // Combine all the buffers

    dbuf result = (dbuf){0};

    result.size = final_size;
    result.data = calloc(final_size, sizeof(char));

    for(size_t i = 0; i < valid_count; i++)
    {
        dbuf* entry = pool[i];

        size_t len = end_at_ptr ? entry->ptr : entry->size;
        memcpy(dbuf_pos(&result), entry->data, len);

        dbuf_advance(&result, len);

        if(free_inputs)
            dbuf_free(entry);
    }

    free(pool);
    return result;
}



void dbuf_free(dbuf* const buffer)
{
    if(buffer != NULL)
    {
        free(buffer->data);

        *buffer = (dbuf){0};
    }
}
