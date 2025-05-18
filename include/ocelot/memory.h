#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct ocl_dbuf   ocl_dbuf;

struct ocl_dbuf
{
    /* Size of the allocated buffer space in bytes */
    size_t size;

    /* Internal allocated buffer */
    char* memory;

    /* Pointer position for dynamic read/write */
    char* ptr;
};

/*!
 * @brief Allocates and initializes a new data buffer
 * @param size Size of the data buffer in bytes
 * @param data Optional data to initialize the buffer with
 * @return The allocated data buffer
 * @note when data is null, the buffer will be zero initialized
 */
ocl_dbuf ocl_dbuf_create(size_t size, void* data);

void ocl_dbuf_export(ocl_dbuf* const buffer, const char* filepath);

void ocl_dbuf_free(ocl_dbuf* const buffer);

bool ocl_dbuf_is_valid(ocl_dbuf* const buffer);

void ocl_dbuf_seek(ocl_dbuf* const buffer, int bytes);

size_t ocl_dbuf_get_ptr_offset(ocl_dbuf* const buffer);


void ocl_dbuf_writens_bytes(ocl_dbuf* const buffer, void* const src, size_t src_size);

void ocl_dbuf_write_bytes(ocl_dbuf* const buffer, void* const src, size_t src_size);

void ocl_dbuf_write_u32(ocl_dbuf* const buffer, uint32_t value);

void ocl_dbuf_write_u32_array(ocl_dbuf* const buffer, uint32_t* const data, size_t count);

ocl_dbuf ocl_dbuf_fullcopy_buffer(ocl_dbuf* const buffer);

ocl_dbuf ocl_dbuf_merge_buffers(ocl_dbuf* const lhs, ocl_dbuf* const rhs, bool combine_from_ptr, bool free_origins);

