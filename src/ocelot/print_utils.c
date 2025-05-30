#include "utils.h"
#include <stdint.h>
#include <stdio.h>

static const char* _HEX_TABLE = "0123456789ABCDEF";

static uint16_t _byte_to_hex(uint8_t byte)
{
    uint16_t lo = _HEX_TABLE[byte & 0xF];
    uint16_t hi = _HEX_TABLE[byte >> 4u];
    return lo | (hi << 8);
}

void phex_u8(uint8_t value)
{
    uint16_t data = _byte_to_hex(value);
    printf("0x%c%c", (char)(data>>8u), (char)(data & 0xFF));
}

void phex_u16(uint16_t value)
{
    uint16_t lo = _byte_to_hex(value & 0xFF);
    uint16_t hi = _byte_to_hex(value >> 8u);
    printf("0x%c%c%c%c",
            (char)(hi>>8u), (char)(hi & 0xFF),
            (char)(lo>>8u), (char)(lo & 0xFF)
    );
}


void phex_u8_array(void* const src, uint32_t count)
{
    if(src == NULL)
    {
        printf("(Null)\n");
        return;
    }

    uint8_t* ptr = (uint8_t*)src;

    for(uint32_t i = 0; i < count; i++)
    {
        phex_u8(ptr[i]);
        putchar((i % 8 == 0 && i != 0) ? '\n' : ' ');
    }

    putchar('\n');
}
