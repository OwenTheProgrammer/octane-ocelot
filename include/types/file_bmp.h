#pragma once
#include "texture.h"
#include "ocelot/dbuf.h"

ocl_dbuf ocl_bmp_encode_raw_texture(ocl_rawTexture texture);

void ocl_bmp_write_raw_texture(ocl_rawTexture texture, const char* filepath);
