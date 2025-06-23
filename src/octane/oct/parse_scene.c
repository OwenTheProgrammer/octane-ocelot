#include "octane/ibuf/ibuf.h"
#include "octane/oct/scene.h"
#include "octane/oct/scene_descriptor.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static void _load_index_buffers(oct_sceneData* const data, oct_sceneDescriptor scene)
{
    //printf("Loading %u index streams..\n", scene.istream_pool_size);
    data->index_stream_count = scene.istream_pool_size;

    data->index_streams = calloc(scene.istream_pool_size, sizeof(oct_indexBuffer));
    for(uint32_t i = 0; i < scene.istream_pool_size; i++)
    {
        data->index_streams[i] = oct_load_index_buffer(scene, i);
        
        printf("[%u]: Loaded %u indices. (off: %u)\n", i, 
               data->index_streams[i].index_count,
               scene.istream_pool[i].buffer_offset
        );
        
    }

    printf("Loaded %u index streams.\n", data->index_stream_count);
}

oct_sceneData oct_load_scene_data(oct_sceneDescriptor scene)
{
    oct_sceneData data = (oct_sceneData){0};

    //Load the index buffers
    _load_index_buffers(&data, scene);

    return data;
}

void oct_free_scene_data(oct_sceneData* const scene)
{
    if(scene != NULL)
    {
        for(uint32_t i = 0; i < scene->index_stream_count; i++)
        {
            oct_free_index_buffer(&scene->index_streams[i]);
        }

        free(scene->index_streams);

        *scene = (oct_sceneData){0};
    }
}
