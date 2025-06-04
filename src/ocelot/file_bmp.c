#include "types/file_bmp.h"
#include "ocelot/dbuf.h"
#include "ocelot/endian.h"
#include "types/texture.h"
#include <stdbool.h>

static const uint32_t _BMP_HEADER_SIZE = 14;
static const uint32_t _BMP_DIB_SIZE = 108;

//"BM"
static const uint16_t _BMP_MAGIC_LE = 0x4D42;

typedef struct _bmp_header      _bmp_header;
typedef struct _bmp_dibHeaderV4 _bmp_dibHeaderV4;

struct _bmp_header
{
    //char magic[2];

    uint32_t file_size;

    // uint16_t reserved;
    // uint16_t reserved;

    uint32_t data_offset;
};

/* Implements BITMAPV4HEADER */
struct _bmp_dibHeaderV4
{
    /* Size of this header in bytes */
    uint32_t header_size;

    /* The bitmap width in pixels (signed integer for some reason) */
    int32_t width_pixels;

    /* The bitmap height in pixels (signed integer for some reason) */
    int32_t height_pixels;

    /* The number of colour planes (always 1) */
    uint16_t color_planes;

    /* The number of bits per pixel (always 32 for rgba32) */
    uint32_t bits_per_pixel;

    /* The compression method used */
    uint32_t compression_type;

    /* The raw bitmap data size in bytes */
    uint32_t data_size;

    /* Horizontal resolution of the image (pixel/meter, signed integer) */
    int32_t horizontal_ppm;

    /* Vertical resolution of the image (pixel/meter, signed integer) */
    int32_t vertical_ppm;

    /* Number of colours in the palett (default 0) */
    uint32_t palette_colors;

    /* Number of important colours used (default 0 since all are important) */
    uint32_t important_colors;

    /* Red channel bit mask in big endian */
    uint32_t red_mask_be;

    /* Green channel bit mask in big endian */
    uint32_t green_mask_be;

    /* Blue channel bit mask in big endian */
    uint32_t blue_mask_be;

    /* Alpha channel bit mask in big endian */
    uint32_t alpha_mask_be;

    /* Always "Win " */
    uint32_t color_space;

    // ciexyztriple[36]

    // red_gamma[4]
    // green_gamma[4]
    // blue_gamma[4]
};

static _bmp_header _init_bmp_header(uint32_t pixel_count)
{
    _bmp_header header = (_bmp_header){0};

    header.data_offset = _BMP_HEADER_SIZE + _BMP_DIB_SIZE;
    header.file_size = header.data_offset + (pixel_count * sizeof(uint32_t));

    return header;
}

static _bmp_dibHeaderV4 _init_bmp_dib(uint32_t width, uint32_t height)
{
    _bmp_dibHeaderV4 dib = (_bmp_dibHeaderV4){0};

    dib.header_size = _BMP_DIB_SIZE;
    dib.width_pixels = width;
    dib.height_pixels = height;
    dib.color_planes = 1;
    dib.bits_per_pixel = 32;
    // BI_BITFIELDS
    dib.compression_type = 3;
    dib.data_size = (width * height) * sizeof(uint32_t);
    dib.horizontal_ppm = 2385;
    dib.vertical_ppm = 2385;
    dib.palette_colors = 0;
    dib.important_colors = 0;

    dib.red_mask_be     = 0x000000FF;
    dib.green_mask_be   = 0x0000FF00;
    dib.blue_mask_be    = 0x00FF0000;
    dib.alpha_mask_be   = 0x00000000;

    //"Win "
    dib.color_space = 0x206E6957;

    return dib;
}

static void _write_bmp_header(ocl_dbuf* const buffer, _bmp_header header)
{
    ocl_dbuf_write_u16(buffer, _BMP_MAGIC_LE);
    ocl_dbuf_write_u32(buffer, header.file_size);
    ocl_dbuf_advance(buffer, 4);
    ocl_dbuf_write_u32(buffer, header.data_offset);
}

static void _write_dib_header(ocl_dbuf* const buffer, _bmp_dibHeaderV4 dib)
{
    ocl_dbuf_write_u32(buffer, dib.header_size);

    ocl_dbuf_write_u32(buffer, dib.width_pixels);
    ocl_dbuf_write_u32(buffer, dib.height_pixels);

    ocl_dbuf_write_u16(buffer, dib.color_planes);
    ocl_dbuf_write_u16(buffer, dib.bits_per_pixel);
    ocl_dbuf_write_u32(buffer, dib.compression_type);

    ocl_dbuf_write_u32(buffer, dib.data_size);

    // TODO: Implement signed integer writing here
    ocl_dbuf_write_u32(buffer, dib.horizontal_ppm);
    ocl_dbuf_write_u32(buffer, dib.vertical_ppm);

    ocl_dbuf_write_u32(buffer, dib.palette_colors);
    ocl_dbuf_write_u32(buffer, dib.important_colors);

    ocl_dbuf_write_u32(buffer, dib.red_mask_be);
    ocl_dbuf_write_u32(buffer, dib.green_mask_be);
    ocl_dbuf_write_u32(buffer, dib.blue_mask_be);
    ocl_dbuf_write_u32(buffer, dib.alpha_mask_be);

    ocl_dbuf_write_u32(buffer, dib.color_space);

    ocl_dbuf_advance(buffer, 36 + 4 + 4 + 4);
}


ocl_dbuf ocl_bmp_encode_raw_texture(ocl_texture texture, bool flip_y)
{
    endian_t prev_ctx = endian_get_current();
    endian_t prev_tgt = endian_get_target();

    endian_set_current(ENDIAN_LITTLE);
    endian_set_target(ENDIAN_LITTLE);

    uint32_t pixel_count = texture.width * texture.height;
    _bmp_header header = _init_bmp_header(pixel_count);
    _bmp_dibHeaderV4 dib = _init_bmp_dib(texture.width, texture.height);

    ocl_dbuf data = ocl_dbuf_create(header.file_size, NULL);

    _write_bmp_header(&data, header);

    _write_dib_header(&data, dib);

    uint32_t* ptr = ocl_dbuf_pos(&data);
    if(!flip_y)
    {
        ocl_texture_flip_y_ext(&texture, ptr);
    }
    else
    {
        ocl_dbuf_write_u8_array(&data, (uint8_t*)texture.data, pixel_count * sizeof(uint32_t));
    }

    endian_set_current(prev_ctx);
    endian_set_target(prev_tgt);

    return data;
}


void ocl_bmp_write_raw_texture(ocl_texture texture, bool flip_y, const char* filepath)
{
    ocl_dbuf data = ocl_bmp_encode_raw_texture(texture, flip_y);
    ocl_dbuf_write(data, filepath);
    ocl_dbuf_free(&data);
}
