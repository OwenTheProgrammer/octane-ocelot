#pragma once
#include "byteconv/endian.h"
#include <stddef.h>
#include <string.h>

extern const size_t _OCT_MAGIC_SIZE;
extern const size_t _OCT_HEADER_SIZE;

extern const char _OCT_MAGIC_LE[8];
extern const char _OCT_MAGIC_BE[8];

endian_t _oct_magic_to_endian(void* const magic);

void _oct_endian_to_magic(endian_t endian, const char** magic);
