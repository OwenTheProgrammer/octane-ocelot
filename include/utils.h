#pragma once

#include <stdint.h>

#define BITMASK(x) ((1 << (x)) - 1)

#define NMASK(value, bits) ((value) & BITMASK(bits))

typedef unsigned int    bool_t;

#define TRUE 1
#define FALSE 0


