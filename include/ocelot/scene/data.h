#pragma once

#include "data/dbuf.h"
#include "octane/ibuf/ibuf.h"
#include "octane/oct/scene_descriptor.h"
#include "octane/vbuf/vbuf.h"
#include <stdint.h>

typedef struct ocl_sceneData    ocl_sceneData;


/* Represents the scene data of an octane world asset */
struct ocl_sceneData
{
    /* == EXTERNAL DATA == */

    oct_sceneDescriptor* scene_descriptor;

    oct_file* file_data;

    /* Vertex buffer file (.vbuf) */
    dbuf vbuf_file;

    /* Index buffer file (.ibuf) */
    dbuf ibuf_file;

    /* Stride of each element in the index buffer file */
    uint32_t ibuf_stride;


    /* == OCTANE VERTEX BUFFERS == */

    uint32_t vertex_buffer_count;

    oct_vertexBuffer* vertex_buffers;


    /* == OCTANE INDEX BUFFERS == */

    uint32_t index_buffer_count;

    oct_indexBuffer* index_buffers;
};

ocl_sceneData ocl_load_scene_data(oct_sceneDescriptor* const scene, oct_file* const oct);

void ocl_free_scene_data(ocl_sceneData* const scene);
