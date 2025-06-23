#pragma once
#include "octane/ibuf/ibuf.h"
#include "octane/oct/scene_descriptor.h"
#include "octane/vbuf/vbuf.h"
#include <stdint.h>

typedef struct oct_sceneData    oct_sceneData;

struct oct_sceneData
{
    uint32_t index_stream_count;

    oct_indexBuffer* index_streams;

    uint32_t vertex_stream_count;

    oct_vertexBuffer* vertex_streams;
};

oct_sceneData oct_load_scene_data(oct_sceneDescriptor scene);

void oct_free_scene_data(oct_sceneData* const scene);
