#include "file_io.h"
#include "endian.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

io_dbuf io_load_file(const char* path)
{
    io_dbuf result = (io_dbuf){0};

    if(path == NULL)
    {
        fprintf(stderr, "Please give a valid file path.\n");
        return result;
    }

    FILE* file = fopen(path, "rb");
    if(file == NULL)
    {
        fprintf(stderr, "Failed to open file \"%s\"\n", path);
        return result;
    }

    // Get the size of the file in bytes
    fseek(file, 0, SEEK_END);
    result.size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Read the files contents to the data buffer
    result.data = calloc(result.size, sizeof(char));
    fread(result.data, sizeof(char), result.size, file);

    fclose(file);

    result.ptr = result.data;
    return result;
}

void io_free_dbuf(io_dbuf* const buffer)
{
    if(buffer == NULL)
        return;

    if(buffer->data != NULL)
        free(buffer->data);

    *buffer = (io_dbuf){0};
}


void io_ptr_advance(io_dbuf* const buffer, size_t bytes)
{
    buffer->ptr += bytes;
}


size_t io_get_ptr_offset(io_dbuf* const buffer)
{
    return (size_t)(buffer->ptr - buffer->data);
}

char* io_read_seek(io_dbuf* const buffer, size_t size)
{
    char* prev = buffer->ptr;
    buffer->ptr += size;

    size_t dist = io_get_ptr_offset(buffer);
    if(dist >= buffer->size)
    {
        fprintf(stderr, "Read went out of bounds. off: %zu | size: %zu.\n", dist, buffer->size);
    }

    return prev;
}


uint16_t io_read_u16(io_dbuf* const buffer)
{
    uint16_t raw = *(uint16_t*)io_read_seek(buffer, sizeof(uint16_t));
    return endian_swap_u16(raw);
}

uint32_t io_read_u32(io_dbuf* const buffer)
{
    uint32_t raw = *(uint32_t*)io_read_seek(buffer, sizeof(uint32_t));
    return endian_swap_u32(raw);
}

float io_read_f32(io_dbuf* const buffer)
{
    uint32_t mem = io_read_u32(buffer);
    return *(float*)&mem;
}

uint32_t io_readns_u32(io_dbuf* const buffer)
{
    uint32_t raw = *(uint32_t*)buffer->ptr;
    return endian_swap_u32(raw);
}

uint32_t io_readns_uvar(io_dbuf* const buffer, size_t stride)
{
    uint32_t raw = *(uint32_t*)buffer->ptr;
    return endian_swap_uvar(raw, stride);
}

uint8_t* io_read_u8_array(io_dbuf* const buffer, size_t count)
{
    uint8_t* mem = calloc(count, sizeof(uint8_t));
    memcpy(mem, buffer->ptr, count);
    io_ptr_advance(buffer, count);
    return mem;
}

uint16_t* io_read_u16_array(io_dbuf* const buffer, size_t count)
{
    uint16_t* data = endian_read_array_u16(buffer->ptr, count);
    io_ptr_advance(buffer, count * sizeof(uint16_t));
    return data;
}

uint32_t* io_read_u32_array(io_dbuf* const buffer, size_t count)
{
    uint32_t* data = endian_read_array_u32(buffer, count);
    io_ptr_advance(buffer, count * sizeof(uint32_t));
    return data;
}

int32_t* io_read_s32_array(io_dbuf* const buffer, size_t count)
{
    uint32_t* mem = io_read_u32_array(buffer, count);
    return (int32_t*)mem;
}

float* io_read_f32_array(io_dbuf* const buffer, size_t count)
{
    float* data = endian_read_array_f32(buffer->ptr, count);
    io_ptr_advance(buffer, count * sizeof(float));
    return data;
}

int32_t* io_read_svar_array(io_dbuf* const buffer, size_t count, size_t stride)
{
    int32_t* data = endian_read_array_svar(buffer->ptr, count, stride);
    io_ptr_advance(buffer, count * stride);
    return data;
}

