#pragma once

#include <stdint.h>

typedef struct ocl_textureBlockBC1  ocl_textureBlockBC1;
typedef struct ocl_rawTexture       ocl_rawTexture;


struct ocl_textureBlockBC1
{
    uint16_t block_min;
    uint16_t block_max;

    uint32_t lut;
};

struct ocl_rawTexture
{
    uint32_t width;
    uint32_t height;

    uint32_t* data;
};

/* == COLOUR CONVERSIONS == */

/* Converts R[15:11] G[10:5] B[4:0] to R[7:0] G[15:8] B[23:16] */
uint32_t ocl_rgb565_to_rgb24(uint16_t value);


uint32_t ocl_lerp_rgb24(uint32_t from, uint32_t to, float t);

/* == BLOCK COMPRESSIONS == */
//https://learn.microsoft.com/en-us/windows/win32/direct3d10/d3d10-graphics-programming-guide-resources-block-compression

void ocl_decode_block_bc1(ocl_textureBlockBC1 block, uint32_t* const result);

