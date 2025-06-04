#pragma once
#include "texture.h"
#include "ocelot/dbuf.h"
#include <stdbool.h>

ocl_dbuf ocl_bmp_encode_raw_texture(ocl_texture texture, bool flip_y);

void ocl_bmp_write_raw_texture(ocl_texture texture, bool flip_y, const char* filepath);
