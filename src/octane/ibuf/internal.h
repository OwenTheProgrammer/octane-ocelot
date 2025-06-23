#pragma once
#include "platform.h"
#include <stdint.h>

/** @brief Calculates the amount of indices an IndexStream has for the current platform.
 *  @param[in] length The `IndexStream/Length` term
 *  @param[in] stride The `IndexBuffer/Width` term
 *  @return The index count
 */
static inline uint32_t _oct_ibuf_get_index_count(uint32_t length, uint32_t stride)
{
    return _platform.pfn_ibuf.get_index_count(length, stride);
}
