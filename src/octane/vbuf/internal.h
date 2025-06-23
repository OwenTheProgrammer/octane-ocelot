#pragma once

#include "octane/oct/enums.h"
#include "platform.h"
#include <stdint.h>


static inline oct_elementType _oct_vbuf_get_element_type(uint32_t enum_value)
{
    return _platform.pfn_vbuf.get_vert_element_type(enum_value);
}
