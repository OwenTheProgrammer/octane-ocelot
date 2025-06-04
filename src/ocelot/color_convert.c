#include "types/texture.h"

static float _lerpf(float from, float to, float t)
{
    return to + t * (from - to);
}

static uint32_t _pack_rgb24_u32(uint32_t r, uint32_t g, uint32_t b)
{
    return r | (g << 8) | (b << 16);
}


uint32_t ocl_rgb565_to_rgb24(uint16_t value)
{
    //R8, G8, B8
    uint32_t r = (value >> 8) & 0xF8;
    uint32_t g = (value >> 3) & 0xFC;
    uint32_t b = (value << 3) & 0xF8;
    return _pack_rgb24_u32(r, g, b);
}


uint32_t ocl_lerp_rgb24(uint32_t from, uint32_t to, float t)
{
    uint8_t r_f = from & 0xFF;
    uint8_t g_f = (from >> 8) & 0xFF;
    uint8_t b_f = (from >> 16) & 0xFF;

    uint8_t r_t = to & 0xFF;
    uint8_t g_t = (to >> 8) & 0xFF;
    uint8_t b_t = (to >> 16) & 0xFF;

    uint32_t r = (uint32_t)_lerpf((float)r_f, (float)r_t, t);
    uint32_t g = (uint32_t)_lerpf((float)g_f, (float)g_t, t);
    uint32_t b = (uint32_t)_lerpf((float)b_f, (float)b_t, t);

    return _pack_rgb24_u32(r, g, b);
}


void ocl_decode_block_bc1(ocl_textureBlockBC1 block, uint32_t* const result)
{
    uint32_t min = ocl_rgb565_to_rgb24(block.block_min);
    uint32_t max = ocl_rgb565_to_rgb24(block.block_max);

    uint32_t lo = ocl_lerp_rgb24(min, max, 1.0/3.0);
    uint32_t hi = ocl_lerp_rgb24(min, max, 2.0/3.0);

    uint32_t table[4] = {min, max, lo, hi};

    for(uint32_t i = 0; i < 16; i++)
    {
        uint32_t idx = (block.lut >> (i*2)) & 0b11;
        result[i] = table[idx];
    }
}
