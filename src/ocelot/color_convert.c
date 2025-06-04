#include "types/texture.h"

static float _lerpf(float from, float to, float t)
{
    return from + t * (to - from);
}

uint32_t ocl_pack_rgb24(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)r) | ((uint32_t)g << 8) | ((uint32_t)b << 16);
}

uint32_t ocl_pack_rgba32(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    return ((uint32_t)r) | ((uint32_t)g << 8) | ((uint32_t)b << 16) | ((uint32_t)a << 24);
}

uint32_t ocl_rgb565_to_rgb24(uint16_t value)
{
    //R8, G8, B8
    uint32_t r = (value >> 11) & 0x001F;
    uint32_t g = (value >> 5) & 0x003F;
    uint32_t b = (value >> 0) & 0x001F;

    r = (r * 527 + 23) >> 6;
    g = (g * 259 + 33) >> 6;
    b = (b * 527 + 23) >> 6;

    return ocl_pack_rgb24(r, g, b);
}

uint16_t ocl_rgb24_to_rgb565(uint32_t rgb_packed)
{
    uint32_t r = (rgb_packed) & 0xFF;
    uint32_t g = (rgb_packed >> 8) & 0xFF;
    uint32_t b = (rgb_packed >> 16) & 0xFF;

    r = (r * 249 + 1014) >> 11;
    g = (g * 253 + 505) >> 10;
    b = (b * 249 + 1014) >> 11;

    return (r << 11) | (g << 5) | b;
}

uint32_t ocl_lerp_rgb24(uint32_t from, uint32_t to, float t)
{
    uint32_t r_f = from & 0xFF;
    uint32_t g_f = (from >> 8) & 0xFF;
    uint32_t b_f = (from >> 16) & 0xFF;

    uint32_t r_t = to & 0xFF;
    uint32_t g_t = (to >> 8) & 0xFF;
    uint32_t b_t = (to >> 16) & 0xFF;

    uint32_t r = (uint32_t)_lerpf((float)r_f, (float)r_t, t);
    uint32_t g = (uint32_t)_lerpf((float)g_f, (float)g_t, t);
    uint32_t b = (uint32_t)_lerpf((float)b_f, (float)b_t, t);

    return ocl_pack_rgb24(r, g, b);
}

