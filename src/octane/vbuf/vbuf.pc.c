
#include "octane/vbuf/vbuf.pc.h"
#include "octane/oct/enums.h"
#include "octane/oct/enums.pc.h"
#include <stdint.h>

oct_elementType pc_oct_vbuf_get_element_type(uint32_t enum_value)
{
    pc_oct_elementType type = (pc_oct_elementType)enum_value;

    switch(type)
    {
        case PC_OCT_ELEMENT_TYPE_FLOAT_ONE:         return OCT_ELEMENT_TYPE_FLOAT_ONE;
        case PC_OCT_ELEMENT_TYPE_FLOAT_TWO:         return OCT_ELEMENT_TYPE_FLOAT_TWO;
        case PC_OCT_ELEMENT_TYPE_FLOAT_THREE:       return OCT_ELEMENT_TYPE_FLOAT_THREE;
        case PC_OCT_ELEMENT_TYPE_FLOAT_FOUR:        return OCT_ELEMENT_TYPE_FLOAT_FOUR;
        case PC_OCT_ELEMENT_TYPE_HALF_TWO_NORM:     return OCT_ELEMENT_TYPE_HALF_TWO_NORM;
        case PC_OCT_ELEMENT_TYPE_HALF_FOUR_NORM:    return OCT_ELEMENT_TYPE_HALF_FOUR_NORM;
        case PC_OCT_ELEMENT_TYPE_UBYTE_FOUR:        return OCT_ELEMENT_TYPE_UBYTE_FOUR;
        case PC_OCT_ELEMENT_TYPE_UBYTE_FOUR_NORM:   return OCT_ELEMENT_TYPE_UBYTE_FOUR_NORM;
        case PC_OCT_ELEMENT_TYPE_HALF_TWO:          return OCT_ELEMENT_TYPE_HALF_TWO;
        case PC_OCT_ELEMENT_TYPE_HALF_FOUR:         return OCT_ELEMENT_TYPE_HALF_FOUR;
        default:
            return OCT_ELEMENT_TYPE_UNKNOWN;
    }
}
