#pragma once

#include "octane/oct/scene.h"
#include <cglm/vec3.h>
#include <stdint.h>

typedef struct oce_model    oce_model;


struct oce_model
{
    uint32_t vertex_count;

    uint32_t index_count;

    vec3* vertex_positions;

    uint32_t* indices;
};

oce_model oce_load_octane_model(oct_sceneDescriptor scene, uint32_t vstream_index, uint32_t istream_index);

void oce_free_model(oce_model* const model);
