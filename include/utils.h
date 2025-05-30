#pragma once

#include <stdint.h>

#define BITMASK(x) ((1 << (x)) - 1)

#define NMASK(value, bits) ((value) & BITMASK(bits))

typedef unsigned int    bool_t;

#define TRUE 1
#define FALSE 0


void phex_u8(uint8_t value);

void phex_u16(uint16_t value);

void phex_u8_array(void* const src, uint32_t count);
