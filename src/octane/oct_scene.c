#include "octane/oct.h"
#include "octane/oct_atoms.h"
#include "octane/oct_scene.h"
#include "oct_scene_internal.h"
#include <stdint.h>
#include <stdlib.h>


oct_sceneDescriptor oct_parse_scene_descriptor(oct_file oct)
{
    oct_sceneDescriptor scene = (oct_sceneDescriptor){0};

    oct_init_atom_name_table(oct);

    _oct_parse_index_stream_pool(&scene, oct);

    _oct_parse_vertex_stream_pool(&scene, oct);

    return scene;
}


void oct_print_scene_descriptor(oct_file oct, oct_sceneDescriptor scene)
{
    for(uint32_t i = 0; i < scene.istream_pool_size; i++)
    {
        oct_print_atom_index_stream(oct, scene.istream_pool[i]);
    }
    for(uint32_t i = 0; i < scene.vstream_pool_size; i++)
    {
        oct_print_atom_vertex_stream(oct, scene.vstream_pool[i]);
    }
}


void oct_free_scene_descriptor(oct_sceneDescriptor* const scene)
{
    if(scene != NULL)
    {
        free(scene->istream_pool);

        for(uint32_t i = 0; i < scene->vstream_pool_size; i++)
        {
            free(scene->vstream_pool[i].elements);
        }
        free(scene->vstream_pool);

        *scene = (oct_sceneDescriptor){0};
    }
}
