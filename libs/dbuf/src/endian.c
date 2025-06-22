#include "data/endian.h"
#include "data/bswap.h"
#include <stdint.h>

/* Masks `_N` bits out of the `_X` value */
#define NMASK(_X, _N) ( (_X) & ((1u<<(_N))-1u) )

/* The current endian value for the library internal runtime */
static endian_t _current = ENDIAN_LITTLE;

/* The target endian value for the library internal runtime */
static endian_t _target = ENDIAN_LITTLE;

/* The print friendly variants of the `endian_t` enum value */
static const char* _ENDIAN_PRINT_TABLE[ENDIAN_MAX];

void endian_set_current(endian_t endian)
{
    _current = endian;
}

void endian_set_target(endian_t endian)
{
    _target = endian;
}

endian_t endian_get_current()
{
    return _current;
}

endian_t endian_get_target()
{
    return _target;
}

int endian_should_convert()
{
    return _current != _target;
}

const char* endian_to_string(endian_t endian)
{
    unsigned int idx = (unsigned int)endian;

    // ENDIAN_UNKNOWN = 0, so a multiplication handles OOB cases.
    return _ENDIAN_PRINT_TABLE[idx * (idx < ENDIAN_MAX)];
}

const char* endian_target_to_string()
{
    return _ENDIAN_PRINT_TABLE[(int)_target];
}

const char* endian_current_to_string()
{
    return _ENDIAN_PRINT_TABLE[(int)_current];
}

uint16_t endian_swap_u16(uint16_t value)
{
    return endian_should_convert() ? bswap_u16(value) : value;
}

uint32_t endian_swap_u32(uint32_t value)
{
    return endian_should_convert() ? bswap_u32(value) : value;
}

uint32_t endian_swap_uvar(uint32_t value, unsigned int stride)
{
    return endian_should_convert() ? bswap_uvar(value, stride) : NMASK(value, stride * 8);
}


uint16_t endian_eval_u16(uint16_t value, endian_t value_endian)
{
    return (value_endian != _current) ? bswap_u16(value) : value;
}


uint32_t endian_load_svar(uint32_t value, unsigned int stride)
{
    // Ensure the value is LE before we work on it (Assuming machine is LE)
    if(endian_get_current() == ENDIAN_BIG)
        value = bswap_uvar(value, stride);

    // Mask potentially unwanted bits (ignored if masking doesnt do anything)
    if(stride < 4)
        value = NMASK(value, stride * 8u);

    // Perform a signature extension
    uint32_t sign_bit = 1u << (stride * 8u - 1u);
    value |= UINT32_MAX * (value & sign_bit);

    // Swap the endian back to BE if thats what we want
    if(endian_get_target() == ENDIAN_BIG)
        value = bswap_u32(value);

    return (uint32_t)value;
}


static const char* _ENDIAN_PRINT_TABLE[ENDIAN_MAX] =
{
    "Unknown",
    "Little",
    "Big"
};

