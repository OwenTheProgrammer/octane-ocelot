#include "data/dbuf.h"
#include "internal.h"
#include <stdint.h>

#define NMASK(_V, _N) ( (_V) & ((1<<(_N))-1) )

static void _vld_f32n(dbuf* const vbuf, float* const dst, uint32_t n)
{
    for(uint32_t i = 0; i < n; i++)
    {
        uint32_t raw = dbuf_read_u32(vbuf);
        dst[i] = *(float*)&raw;
    }
}

static float _f16_to_f32(uint16_t half)
{
    uint32_t sign = half << 16;
    uint32_t exp  = (half >> 10) & 0x1F;
    uint32_t frac = half & 0x3FF;

    uint32_t mem = sign & (1<<31);
    mem |= (exp + (0x7F - 15)) << 23;
    mem |= frac << 13;

    return *(float*)&mem;
}

static int32_t _sign_extend(uint32_t value, uint32_t sign_bit)
{
    uint32_t mask = 1 << sign_bit;
    uint32_t ext = value | (-(value & mask));
    return (int32_t)ext;
}

void _oct_vload_float(dbuf* const vbuf, unsigned int count, float* const dst)
{
    for(uint32_t i = 0; i < count; i++)
    {
        uint32_t raw = dbuf_read_u32(vbuf);
        dst[i] = *(float*)&raw;
    }
}


void _oct_vload_short2n(dbuf* const vbuf, float* const dst)
{
    for(uint32_t i = 0; i < 2; i++)
    {
        int16_t raw = (int16_t)dbuf_read_u16(vbuf);
        dst[i] = (double)raw / 32767.0;
    }
}


void _oct_vload_short4n(dbuf* const vbuf, float* const dst)
{
    for(uint32_t i = 0; i < 4; i++)
    {
        int16_t raw = (int16_t)dbuf_read_u16(vbuf);
        dst[i] = (double)raw / 32767.0;
    }
}


void _oct_vload_pvec3(dbuf* const vbuf, float* const dst)
{
    // S11_S11_S10 Layout
    // zzzzzzzzzz yyyyyyyyyyy xxxxxxxxxxx
    uint32_t raw = dbuf_read_u32(vbuf);

    static const uint32_t bt[3] = {11, 11, 10};

    for(uint32_t i = 0; i < 3; i++)
    {
        uint32_t bits = bt[i];
        uint32_t mask = (1 << bits) - 1;
        int32_t raw_sgn = _sign_extend(raw & mask, bits-1);
        raw >>= bits;

        double sgn = (double)raw_sgn / (double)(mask>>1);
        dst[i] = (float)sgn;
    }
}

void _oct_vload_half2(dbuf* const vbuf, float* const dst)
{
    for(uint32_t i = 0; i < 2; i++)
    {
        uint16_t raw = dbuf_read_u16(vbuf);
        dst[i] = _f16_to_f32(raw);
    }
}

void _oct_vload_half4(dbuf* const vbuf, float* const dst)
{
    for(uint32_t i = 0; i < 4; i++)
    {
        uint16_t raw = dbuf_read_u16(vbuf);
        dst[i] = _f16_to_f32(raw);
    }
}

