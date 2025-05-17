#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef enum
{
    ENDIAN_BIG,
    ENDIAN_LITTLE,
    ENDIAN_UNKNOWN,
    ENDIAN_MAX,
} endian_t;

extern const char* _ENDIAN_PRINT_TABLE[ENDIAN_MAX];

/* Set the target global endian type (the endian you wish to have) */
void set_endian_target(endian_t e);

/* Gets the target global endian type */
endian_t get_endian_target();

/* Set the global context endian type (the endian files are stored with) */
void set_endian_context(endian_t e);

/* Gets the global context endian type */
endian_t get_endian_context();

/* Returns true if the endian context does not match the endian target */
bool io_different_endian();

/* Swap the endianness of a uint16 value */
uint16_t endian_swap_u16(uint16_t v);

/* Swaps the endianness of a uint32 value */
uint32_t endian_swap_u32(uint32_t v);

/* Swaps the endianness of an unsigned value with variable stride */
uint32_t endian_swap_uvar(uint32_t v, unsigned int stride);

/* Reads an array of uint16 values */
uint16_t* endian_read_array_u16(void* const ptr, size_t count);

/* Reads an array of uint32 values */
uint32_t* endian_read_array_u32(void* const ptr, size_t count);

/* Reads an array of float32 values */
float* endian_read_array_f32(void* const ptr, size_t count);

/* Reads an array of variable stride signed values */
int32_t* endian_read_array_svar(void* const ptr, size_t count, size_t stride);

