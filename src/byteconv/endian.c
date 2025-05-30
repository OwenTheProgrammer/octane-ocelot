#include "byteconv/endian.h"
#include "byteconv/bswap.h"
#include "utils.h"
#include <stdint.h>

static endian_t _current = ENDIAN_LITTLE;
static endian_t _target = ENDIAN_LITTLE;

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


int32_t endian_load_svar(uint32_t value, unsigned int stride)
{
    if(endian_get_current() == ENDIAN_BIG)
        value = bswap_uvar(value, stride);

    if(stride < 4)
        value = NMASK(value, stride * 8u);

    uint32_t sign_bit = 1u << (stride * 8u - 1u);
    value |= UINT32_MAX * (value & sign_bit);

    if(endian_get_target() == ENDIAN_BIG)
        value = bswap_u32(value);

    return (int32_t)value;
}


const char* _ENDIAN_PRINT_TABLE[ENDIAN_MAX] =
{
    "Unknown",
    "Little",
    "Big"
};

