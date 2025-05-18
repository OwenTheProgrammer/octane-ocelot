#pragma once
#include <stdint.h>
#include <stddef.h>

typedef struct io_dbuf  io_dbuf;

struct io_dbuf
{
    /* Allocated data buffer */
    char* data;

    /* Size of the buffer in bytes */
    size_t size;

    /* Pointer into the data buffer */
    char* ptr;
};

/* Read the data of a file into a data buffer */
io_dbuf io_load_file(const char* path);

/* Advance the pointer forward N bytes */
void io_ptr_advance(io_dbuf* const buffer, size_t bytes);

/* The current pointer position relative to the data buffers start in bytes */
size_t io_get_ptr_offset(io_dbuf* const buffer);

/* Read data and seek forward */
char* io_read_seek(io_dbuf* const buffer, size_t size);

/* Read a uint16 value */
uint16_t io_read_u16(io_dbuf* const buffer);

/* Read a uint32 value */
uint32_t io_read_u32(io_dbuf* const buffer);

/* Read a float32 value */
float io_read_f32(io_dbuf* const buffer);

/* Read a uint32 value but dont seek forward */
uint32_t io_readns_u32(io_dbuf* const buffer);

/* Read an unsigned variable stride int, but dont seek forward */
uint32_t io_readns_uvar(io_dbuf* const buffer, size_t stride);

/* Read an array of uint8 values */
uint8_t* io_read_u8_array(io_dbuf* const buffer, size_t count);

/* Read an array of uint16 values */
uint16_t* io_read_u16_array(io_dbuf* const buffer, size_t count);

/* Read an array of uint32 values */
uint32_t* io_read_u32_array(io_dbuf* const buffer, size_t count);

/* Read an array of int32 values */
int32_t* io_read_s32_array(io_dbuf* const buffer, size_t count);

/* Read an array of variable stride, signed integers */
int32_t* io_read_svar_array(io_dbuf* const buffer, size_t count, size_t stride);

/* Read an array of float32 values */
float* io_read_f32_array(io_dbuf* const buffer, size_t count);

/* Free the allocated memory of a data buffer */
void io_free_dbuf(io_dbuf* const buffer);
