/* oct format enums specific to the PC platform */
#pragma once

#ifndef __cplusplus

typedef enum pc_oct_elementType pc_oct_elementType;

#endif

/* Implements the pc graphics types used by vertex elements and material properties */
enum pc_oct_elementType
{
    PC_OCT_ELEMENT_TYPE_FLOAT_ONE       = 0,
    PC_OCT_ELEMENT_TYPE_FLOAT_TWO       = 1,
    PC_OCT_ELEMENT_TYPE_FLOAT_THREE     = 2,
    PC_OCT_ELEMENT_TYPE_FLOAT_FOUR      = 3,
    PC_OCT_ELEMENT_TYPE_HALF_TWO_NORM   = 21,
    PC_OCT_ELEMENT_TYPE_HALF_FOUR_NORM  = 23,
    PC_OCT_ELEMENT_TYPE_UBYTE_FOUR      = 24,
    PC_OCT_ELEMENT_TYPE_UBYTE_FOUR_NORM = 25,
    PC_OCT_ELEMENT_TYPE_HALF_TWO        = 44,
    PC_OCT_ELEMENT_TYPE_HALF_FOUR       = 45,
    PC_OCT_ELEMENT_TYPE_UNUSED          = 84,
};
