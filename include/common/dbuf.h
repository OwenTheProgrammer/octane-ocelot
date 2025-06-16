#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct dbuf dbuf;

struct dbuf
{
    /* The allocated memory */
    void* data;

    /* The size of the buffer in bytes */
    size_t size;

    /* The current pointer offset into the buffer */
    size_t ptr;
};

dbuf dbuf_create(size_t size, void* const data);

dbuf dbuf_load(const char* filepath);

bool dbuf_is_valid(dbuf* const buffer);

void dbuf_write(dbuf buffer, const char* filepath);


void dbuf_ensure_capacity(dbuf* const buffer, size_t bytes);

void dbuf_advance(dbuf* const buffer, size_t bytes);

void* dbuf_pos(dbuf* const buffer);

void dbuf_write_data(dbuf* const buffer, void* const src, size_t bytes, bool advance);

dbuf dbuf_merge(bool end_at_ptr, bool free_inputs, size_t count, ...);


uint16_t dbuf_read_u16(dbuf* const buffer);

uint32_t dbuf_read_u32(dbuf* const buffer);

uint32_t dbuf_read_uvar(dbuf* const buffer, unsigned int stride);

uint8_t* dbuf_read_u8_array(dbuf* const buffer, size_t count);

uint16_t* dbuf_read_u16_array(dbuf* const buffer, size_t count);

uint32_t* dbuf_read_u32_array(dbuf* const buffer, size_t count);

uint32_t* dbuf_read_uvar_array(dbuf* const buffer, size_t count, unsigned int stride);

int32_t* dbuf_read_svar_array(dbuf* const buffer, size_t count, unsigned int stride);



void dbuf_write_u16(dbuf* const buffer, uint16_t value);

void dbuf_write_u32(dbuf* const buffer, uint32_t value);

void dbuf_write_uvar(dbuf* const buffer, uint32_t value, unsigned int stride);

void dbuf_write_u8_array(dbuf* const buffer, uint8_t* const src, size_t count);

void dbuf_write_u16_array(dbuf* const buffer, uint16_t* const src, size_t count);

void dbuf_write_u32_array(dbuf* const buffer, uint32_t* const src, size_t count);

void dbuf_write_uvar_array(dbuf* const buffer, uint32_t* const src, size_t count, unsigned int stride);

void dbuf_free(dbuf* const buffer);
