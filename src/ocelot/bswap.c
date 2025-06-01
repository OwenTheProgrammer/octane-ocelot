#include "ocelot/bswap.h"

uint16_t bswap_u16(uint16_t src)
{
    uint16_t r = 0;
    r |= (src & 0x00FF) << 8;
    r |= (src & 0xFF00) >> 8;
    return r;
}

uint32_t bswap_u32(uint32_t src)
{
    uint32_t r = 0;
    r |= (src & 0x000000FF) << 24;
    r |= (src & 0x0000FF00) << 8;
    r |= (src & 0x00FF0000) >> 8;
    r |= (src & 0xFF000000) >> 24;
    return r;
}

uint32_t bswap_uvar(uint32_t src, unsigned int stride)
{
    uint32_t value = bswap_u32(src);
    return value >> (32 - 8 * stride);
}
