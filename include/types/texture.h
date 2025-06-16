#pragma once

#include "common/dbuf.h"
#include <stdint.h>

typedef struct ocl_textureBlockBC1  ocl_textureBlockBC1;
typedef struct ocl_texture       ocl_texture;


struct ocl_textureBlockBC1
{
    uint16_t block_min;
    uint16_t block_max;

    uint32_t lut;
};

struct ocl_texture
{
    uint32_t width;
    uint32_t height;

    /* Pixel data is stored left to right, top to bottom (DX9 Style) */
    uint32_t* data;
};

ocl_texture ocl_texture_create(uint32_t width, uint32_t height, uint32_t* const data);

uint32_t* ocl_texture_get_pixel_addr(ocl_texture texture, uint32_t pixel_x, uint32_t pixel_y);

void ocl_texture_flip_y(ocl_texture* const texture);

void ocl_texture_flip_y_ext(ocl_texture* const texture, uint32_t* const dst);

void ocl_texture_free(ocl_texture* const texture);

/* == COLOUR CONVERSIONS == */

uint32_t ocl_pack_rgb24(uint8_t r, uint8_t g, uint8_t b);

uint32_t ocl_pack_rgba32(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

/* Converts RGB24 (R[7:0] G[15:8] B[23:16]) to RGB565 (R[15:11] G[10:5] B[4:0]) */
uint16_t ocl_rgb24_to_rgb565(uint32_t rgb_packed);

/* Converts RGB565 (R[15:11] G[10:5] B[4:0]) to RGB24 (R[7:0] G[15:8] B[23:16]) */
uint32_t ocl_rgb565_to_rgb24(uint16_t value);


uint32_t ocl_lerp_rgb24(uint32_t from, uint32_t to, float t);

/* == BLOCK COMPRESSIONS == */
//https://learn.microsoft.com/en-us/windows/win32/direct3d10/d3d10-graphics-programming-guide-resources-block-compression

void ocl_decode_block_bc1(ocl_textureBlockBC1 block, uint32_t result[16]);


ocl_texture ocl_decode_texture_bc1(dbuf buffer);
