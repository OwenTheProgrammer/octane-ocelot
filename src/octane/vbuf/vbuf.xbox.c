#include "octane/vbuf/vbuf.xbox.h"
#include "octane/oct/enums.h"
#include "octane/oct/enums.xbox.h"
#include <stdint.h>

oct_elementType xb_oct_vbuf_get_element_type(uint32_t enum_value)
{
    xb_oct_elementType type = (xb_oct_elementType)enum_value;
    switch(type)
    {
        case XBOX_OCT_ELEMENT_TYPE_FLOAT_ONE:       return OCT_ELEMENT_TYPE_FLOAT_ONE;
        case XBOX_OCT_ELEMENT_TYPE_FLOAT_TWO:       return OCT_ELEMENT_TYPE_FLOAT_TWO;
        case XBOX_OCT_ELEMENT_TYPE_FLOAT_THREE:     return OCT_ELEMENT_TYPE_FLOAT_THREE;
        case XBOX_OCT_ELEMENT_TYPE_FLOAT_FOUR:      return OCT_ELEMENT_TYPE_FLOAT_FOUR;
        case XBOX_OCT_ELEMENT_TYPE_INT_ONE:         return OCT_ELEMENT_TYPE_INT_ONE;
        case XBOX_OCT_ELEMENT_TYPE_HALF_TWO_NORM:   return OCT_ELEMENT_TYPE_HALF_TWO_NORM;
        case XBOX_OCT_ELEMENT_TYPE_HALF_FOUR_NORM:  return OCT_ELEMENT_TYPE_HALF_FOUR_NORM;
        case XBOX_OCT_ELEMENT_TYPE_UBYTE_FOUR:      return OCT_ELEMENT_TYPE_UBYTE_FOUR;
        case XBOX_OCT_ELEMENT_TYPE_UBYTE_FOUR_NORM: return OCT_ELEMENT_TYPE_UBYTE_FOUR_NORM;
        case XBOX_OCT_ELEMENT_TYPE_PVEC_THREE:      return OCT_ELEMENT_TYPE_PVEC_THREE;
        case XBOX_OCT_ELEMENT_TYPE_HALF_TWO:        return OCT_ELEMENT_TYPE_HALF_TWO;
        case XBOX_OCT_ELEMENT_TYPE_HALF_FOUR:       return OCT_ELEMENT_TYPE_HALF_FOUR;
        default:
            return OCT_ELEMENT_TYPE_UNKNOWN;
    }
}
