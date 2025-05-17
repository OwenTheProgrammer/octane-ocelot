#pragma once


#define _OCT_BITMASK(x) ((1u<<(x))-1u)

#define _OCT_MASK_N(v, x) ((v) & _OCT_BITMASK(x))

#define _OCT_TYPEOF(x) typeof(x)
