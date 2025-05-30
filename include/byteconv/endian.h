#pragma once
#include <stdint.h>

typedef enum
{
    ENDIAN_UNKNOWN  = 0,
    ENDIAN_LITTLE   = 1,
    ENDIAN_BIG      = 2,
    ENDIAN_MAX
} endian_t;

extern const char* _ENDIAN_PRINT_TABLE[ENDIAN_MAX];


void endian_set_current(endian_t endian);

void endian_set_target(endian_t endian);

endian_t endian_get_current();

endian_t endian_get_target();


int endian_should_convert();


uint16_t endian_swap_u16(uint16_t value);

uint32_t endian_swap_u32(uint32_t value);

uint32_t endian_swap_uvar(uint32_t value, unsigned int stride);


uint16_t endian_eval_u16(uint16_t value, endian_t value_endian);


int32_t endian_load_svar(uint32_t value, unsigned int stride);
