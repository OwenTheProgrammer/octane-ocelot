#pragma once
#include "texture.h"
#include "common/dbuf.h"
#include <stdbool.h>

dbuf ocl_bmp_encode_raw_texture(ocl_texture texture, bool flip_y);

void ocl_bmp_write_raw_texture(ocl_texture texture, bool flip_y, const char* filepath);
