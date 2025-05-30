#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct ocl_dbuf ocl_dbuf;

struct ocl_dbuf
{
    /* The allocated memory */
    void* data;

    /* The size of the buffer in bytes */
    size_t size;

    /* The current pointer offset into the buffer */
    size_t ptr;
};

ocl_dbuf ocl_dbuf_create(size_t size, void* const data);

ocl_dbuf ocl_dbuf_load(const char* filepath);

void ocl_dbuf_write(ocl_dbuf buffer, const char* filepath);


void ocl_dbuf_ensure_capacity(ocl_dbuf* const buffer, size_t bytes);

void ocl_dbuf_advance(ocl_dbuf* const buffer, size_t bytes);

void* ocl_dbuf_pos(ocl_dbuf* const buffer);

void ocl_dbuf_write_data(ocl_dbuf* const buffer, void* const src, size_t bytes, bool advance);

ocl_dbuf ocl_dbuf_merge(bool end_at_ptr, bool free_inputs, size_t count, ...);


uint16_t ocl_dbuf_read_u16(ocl_dbuf* const buffer);

uint32_t ocl_dbuf_read_u32(ocl_dbuf* const buffer);

uint32_t ocl_dbuf_read_uvar(ocl_dbuf* const buffer, unsigned int stride);

uint8_t* ocl_dbuf_read_u8_array(ocl_dbuf* const buffer, size_t count);

uint16_t* ocl_dbuf_read_u16_array(ocl_dbuf* const buffer, size_t count);

uint32_t* ocl_dbuf_read_u32_array(ocl_dbuf* const buffer, size_t count);

int32_t* ocl_dbuf_read_svar_array(ocl_dbuf* const buffer, size_t count, unsigned int stride);



void ocl_dbuf_write_u16(ocl_dbuf* const buffer, uint16_t value);

void ocl_dbuf_write_u32(ocl_dbuf* const buffer, uint32_t value);

void ocl_dbuf_write_uvar(ocl_dbuf* const buffer, uint32_t value, unsigned int stride);

void ocl_dbuf_write_u8_array(ocl_dbuf* const buffer, uint8_t* const src, size_t count);

void ocl_dbuf_write_u16_array(ocl_dbuf* const buffer, uint16_t* const src, size_t count);

void ocl_dbuf_write_u32_array(ocl_dbuf* const buffer, uint32_t* const src, size_t count);

void ocl_dbuf_write_uvar_array(ocl_dbuf* const buffer, uint32_t* const src, size_t count, unsigned int stride);

void ocl_dbuf_free(ocl_dbuf* const buffer);
