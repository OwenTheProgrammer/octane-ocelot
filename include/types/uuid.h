#pragma once
#include <stdint.h>

/* 128-bit UUID value */
typedef uint32_t    uuid_t[4];

void uuid_from_bytes(uuid_t dst, void* const src);
