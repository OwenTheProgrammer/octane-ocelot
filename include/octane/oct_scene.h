#pragma once

#include "octane/oct.h"
#include "octane/oct_atoms.h"
#include <stdint.h>
typedef struct oct_sceneDescriptor  oct_sceneDescriptor;

struct oct_sceneDescriptor
{
    /* IndexStreamPool */

    uint32_t istream_pool_size;

    oct_indexStreamAtom* istream_pool;
};


oct_sceneDescriptor oct_parse_scene_descriptor(oct_file oct);
