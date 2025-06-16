#pragma once
#include <stdint.h>

uint16_t bswap_u16(uint16_t src);

uint32_t bswap_u32(uint32_t src);

uint32_t bswap_uvar(uint32_t src, unsigned int stride);

