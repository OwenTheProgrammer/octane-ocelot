
#include "types/file_bmp.h"
#include "types/texture.h"
int main()
{
    uint32_t pixels[4] =
    {
        0x7F0000FF,
        0x7F00FF00,
        0xFFFF0000,
        0xFFFFFFFF
    };

    ocl_rawTexture texture = (ocl_rawTexture){0};

    texture.width = 2;
    texture.height = 2;
    texture.data = pixels;

    ocl_bmp_write_raw_texture(texture, "bin/output.bmp");

    return 0;
}
