#pragma once
#include "data/dbuf.h"

/* Loads `OCT_ELEMENT_TYPE_FLOAT_XXXX` */
void _oct_vload_float(dbuf* const vbuf, unsigned int count, float* const dst);


/* Loads `OCT_ELEMENT_TYPE_SHORT_TWO_NORM` */
void _oct_vload_short2n(dbuf* const vbuf, float* const dst);

/* Loads `OCT_ELEMENT_TYPE_SHORT_FOUR_NORM` */
void _oct_vload_short4n(dbuf* const vbuf, float* const dst);


/* Loads `OCT_ELEMENT_TYPE_PVEC_THREE` */
void _oct_vload_pvec3(dbuf* const vbuf, float* const dst);


/* Loads `OCT_ELEMENT_TYPE_HALF_TWO` */
void _oct_vload_half2(dbuf* const vbuf, float* const dst);

/* Loads `OCT_ELEMENT_TYPE_HALF_FOUR` */
void _oct_vload_half4(dbuf* const vbuf, float* const dst);

