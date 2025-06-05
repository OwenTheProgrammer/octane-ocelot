#pragma once

#define BITMASK(x) ((1 << (x)) - 1)

#define NMASK(value, bits) ((value) & BITMASK(bits))

