
#include "data/dbuf.h"
#include "data/endian.h"
#include "types/file_bmp.h"
#include "types/texture.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>


static void _export_uv_bmp()
{
    ocl_texture texture = ocl_texture_create(256, 256, NULL);

    for(uint32_t y = 0; y < 256; y++)
    {
        for(uint32_t x = 0; x < 256; x++)
        {
            uint32_t* addr = ocl_texture_get_pixel_addr(texture, x, y);
            *addr = ocl_pack_rgb24(x, y, 0);
        }
    }

    ocl_bmp_write_raw_texture(texture, false, "bin/assets/uv.bmp");

    ocl_bmp_write_raw_texture(texture, true, "bin/assets/uv_flipped.bmp");

    ocl_texture_free(&texture);
}

static void _export_bc1_bmp()
{

    // BLOCK 0

    uint16_t b0_min = ocl_rgb24_to_rgb565(ocl_pack_rgb24(0, 0, 0));
    uint16_t b0_max = ocl_rgb24_to_rgb565(ocl_pack_rgb24(255, 0, 0));

    //00=min | 01=max | 10=66% min + 33% max | 11=33% min + 66% max
    //top line min, line 2 min to max, line 3 max to min, bottom line max
    uint32_t b0_lut = 0b01010101001011010111100000000000;

    ocl_textureBlockBC1 block =
    {
        .block_min = b0_min,
        .block_max = b0_max,
        .lut = b0_lut
    };

    uint32_t bdata[16];

    ocl_decode_block_bc1(block, bdata);
    ocl_texture texture = ocl_texture_create(4, 4, bdata);

    ocl_bmp_write_raw_texture(texture, true, "bin/assets/bc1.bmp");
    ocl_texture_free(&texture);
}

static int _export_tstream()
{
    dbuf tstream = dbuf_load("bin/assets/oilrig_32.tstream");

    if(!dbuf_is_valid(&tstream))
        return EXIT_FAILURE;

    endian_set_current(ENDIAN_BIG);
    endian_set_target(ENDIAN_LITTLE);

    ocl_texture texture = ocl_decode_texture_bc1(tstream);
    dbuf_free(&tstream);

    ocl_bmp_write_raw_texture(texture, false, "bin/32.bmp");
    ocl_texture_free(&texture);

    return EXIT_SUCCESS;
}


int main()
{

    //_export_uv_bmp();

    //_export_bc1_bmp();

    return _export_tstream();

    //return 0;
}

