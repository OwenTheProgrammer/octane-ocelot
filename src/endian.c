#include "endian.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static endian_t _target_endian = ENDIAN_LITTLE;
static endian_t _ctx_endian = ENDIAN_LITTLE;

void set_endian_target(endian_t e)
{
    if(e == ENDIAN_UNKNOWN)
    {
        fprintf(stderr, "Unknown endian type is invalid.\n");
        return;
    }

    _target_endian = e;
    printf("Endian Target: %s\n", _ENDIAN_PRINT_TABLE[_target_endian]);
}

void set_endian_context(endian_t e)
{
    if(e == ENDIAN_UNKNOWN)
    {
        fprintf(stderr, "Unknown endian type is invalid.\n");
        return;
    }

    _ctx_endian = e;
    printf("Endian Context: %s\n", _ENDIAN_PRINT_TABLE[_ctx_endian]);
}

static uint16_t _eswap_u16(uint16_t v)
{
    return (v >> 8) | (v << 8);
}

static uint32_t _eswap_u32(uint32_t v)
{
    // Not efficient, I dont care.
    uint32_t r = 0;
    r |= (v & 0x000000FF) << 24;
    r |= (v & 0x0000FF00) << 8;
    r |= (v & 0x00FF0000) >> 8;
    r |= (v & 0xFF000000) >> 24;
    return r;
}

uint32_t endian_swap_uvar(uint32_t v, unsigned int stride)
{
    uint32_t shift = 32 - stride * 8;
    uint32_t mask = _OCT_BITMASK(stride * 8);
    uint32_t swap = (_eswap_u32(v) >> shift) & mask;
    return swap;
}

static int32_t _esignextend_svar(uint32_t v, size_t stride)
{
    uint32_t sign_bit = 1 << (stride * 8 - 1);
    uint32_t ext = 0xFFFFFFFF * (v & sign_bit);
    return (int32_t)(v | ext);
}

endian_t get_endian_target()
{
    return _target_endian;
}
endian_t get_endian_context()
{
    return _ctx_endian;
}
bool io_different_endian()
{
    return _ctx_endian != _target_endian;
}

uint16_t endian_swap_u16(uint16_t v)
{
    return (_ctx_endian == _target_endian) ? v : _eswap_u16(v);
}

uint32_t endian_swap_u32(uint32_t v)
{
    return (_ctx_endian == _target_endian) ? v : _eswap_u32(v);
}

uint16_t* endian_read_array_u16(void* const ptr, size_t count)
{
    if(ptr == NULL || count == 0)
        return NULL;

    uint16_t* result = calloc(count, sizeof(uint16_t));
    memcpy(result, ptr, sizeof(uint16_t) * count);

    if(_ctx_endian == _target_endian)
        return result;

    for(size_t i = 0; i < count; i++)
    {
        result[i] = _eswap_u16(result[i]);
    }

    return result;
}

uint32_t* endian_read_array_u32(void* const ptr, size_t count)
{
    if(ptr == NULL || count == 0)
        return NULL;

    uint32_t* result = calloc(count, sizeof(uint32_t));
    memcpy(result, ptr, sizeof(uint32_t) * count);

    if(_ctx_endian == _target_endian)
        return result;

    for(size_t i = 0; i < count; i++)
    {
        result[i] = _eswap_u32(result[i]);
    }

    return result;
}

float* endian_read_array_f32(void* const ptr, size_t count)
{
    if(ptr == NULL || count == 0)
        return NULL;

    float* result = calloc(count, sizeof(float));
    memcpy(result, ptr, sizeof(float) * count);

    if(_ctx_endian == _target_endian)
        return result;

    for(size_t i = 0; i < count; i++)
    {
        uint32_t mem = *(uint32_t*)&result[i];
        mem = _eswap_u32(mem);
        result[i] = *(float*)&mem;
    }

    return result;
}


int32_t* endian_read_array_svar(void* const ptr, size_t count, size_t stride)
{
    if(ptr == NULL || count == 0 || stride == 0)
        return NULL;

    int32_t* result = calloc(count, count * sizeof(int32_t));

    uint32_t shift = 32 - stride * 8;
    uint32_t mask = _OCT_BITMASK(stride * 8);

    for(size_t i = 0; i < count; i++)
    {
        void* off = ptr + stride * i;
        uint32_t mem = *(uint32_t*)off;

        if(_ctx_endian != _target_endian)
            mem = _eswap_u32(mem) >> shift;

        result[i] = _esignextend_svar(mem & mask, stride);
    }

    return result;
}


const char* _ENDIAN_PRINT_TABLE[ENDIAN_MAX] =
{
    "BIG",
    "LITTLE",
    "UNKNOWN"
};
