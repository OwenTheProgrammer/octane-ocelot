#include "types/texture.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static ocl_textureBlockBC1 _read_block_bc1(ocl_dbuf* const buffer)
{
    ocl_textureBlockBC1 block = (ocl_textureBlockBC1){0};

    block.block_min = ocl_dbuf_read_u16(buffer);
    block.block_max = ocl_dbuf_read_u16(buffer);

    block.lut = ocl_dbuf_read_u32(buffer);

    return block;
}

ocl_texture ocl_texture_create(uint32_t width, uint32_t height, uint32_t* const data)
{
    ocl_texture texture = (ocl_texture){0};

    texture.width = width;
    texture.height = height;

    texture.data = calloc(width * height, sizeof(uint32_t));

    if(data != NULL)
        memcpy(texture.data, data, width * height * sizeof(uint32_t));

    return texture;
}


uint32_t* ocl_texture_get_pixel_addr(ocl_texture texture, uint32_t pixel_x, uint32_t pixel_y)
{
    uint32_t offset = texture.width * pixel_y + pixel_x;
    return &texture.data[offset];
}


void ocl_decode_block_bc1(ocl_textureBlockBC1 block, uint32_t result[16])
{
    uint32_t min = ocl_rgb565_to_rgb24(block.block_min);
    uint32_t max = ocl_rgb565_to_rgb24(block.block_max);

    uint32_t lo = ocl_lerp_rgb24(min, max, 1.0/3.0);
    uint32_t hi = ocl_lerp_rgb24(min, max, 2.0/3.0);

    uint32_t table[4] = {min, max, lo, hi};

    for(uint32_t i = 0; i < 16; i++)
    {
        uint32_t idx = (block.lut >> (i*2)) & 0b11;
        result[i] = table[idx];
    }
}


ocl_texture ocl_decode_texture_bc1(ocl_dbuf buffer)
{
    ocl_texture texture = (ocl_texture){0};

    texture.width = 512;
    texture.height = 512;

    texture.data = calloc(texture.width * texture.height, sizeof(uint32_t));

    for(uint32_t y = 0; y < texture.height; y += 4)
    {
        for(uint32_t x = 0; x < texture.width; x += 4)
        {
            ocl_textureBlockBC1 encoded = _read_block_bc1(&buffer);

            uint32_t block[16];
            ocl_decode_block_bc1(encoded, block);

            for(uint32_t i = 0; i < 4; i++)
            {
                memcpy(&texture.data[texture.width * (y+i) + x], &block[i*4], sizeof(uint32_t) * 4);
            }
        }
    }

    return texture;
}


void ocl_texture_flip_y_ext(ocl_texture* const texture, uint32_t* const dst)
{
    for(uint32_t y = 0; y < texture->height; y++)
    {
        uint32_t y_inv = (texture->height-1) - y;

        uint32_t* src_ptr = &texture->data[texture->width * y_inv];
        uint32_t* dst_ptr = &dst[texture->width * y];

        memcpy(dst_ptr, src_ptr, texture->width * sizeof(uint32_t));
    }
}


void ocl_texture_flip_y(ocl_texture* const texture)
{
    uint32_t* result = calloc(texture->width * texture->height, sizeof(uint32_t));

    ocl_texture_flip_y_ext(texture, result);

    free(texture->data);
    texture->data = result;
}


void ocl_texture_free(ocl_texture* const texture)
{
    if(texture != NULL)
    {
        free(texture->data);

        *texture = (ocl_texture){0};
    }
}
