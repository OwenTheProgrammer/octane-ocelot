#include "octane/oct.h"
#include "octane/oct_atoms.h"
#include "octane/oct_scene.h"
#include <stdint.h>
#include <stdlib.h>

oct_sceneDescriptor oct_parse_scene_descriptor(oct_file oct)
{
    oct_sceneDescriptor scene = (oct_sceneDescriptor){0};

    oct_init_atom_name_table(oct);

    // Find the IndexStreamPool node
    uint32_t pool_index = oct_get_atom_node_index(oct, _oct_ant.index_stream_pool);
    uint32_t pool_level = oct.data_tree[pool_index++].header.tree_level;

    uint32_t* stream_table = calloc(UINT16_MAX, sizeof(uint32_t));

    uint32_t count = 0;

    while(oct.data_tree[pool_index].header.tree_level > pool_level)
    {
        oct_atomNode node = oct.data_tree[pool_index];

        if(node.st_key == _oct_ant.index_stream)
            stream_table[count++] = pool_index;

        pool_index++;
    }

    scene.istream_pool_size = count;
    scene.istream_pool = calloc(scene.istream_pool_size, sizeof(oct_indexStreamAtom));

    stream_table[count++] = pool_index;
    stream_table = realloc(stream_table, count * sizeof(uint32_t));

    for(uint32_t i = 0; i < scene.istream_pool_size; i++)
    {
        scene.istream_pool[i] = oct_atom_read_index_stream(oct, stream_table[i], stream_table[i+1]);
    }

    free(stream_table);
    return scene;
}


void oct_print_scene_descriptor(oct_file oct, oct_sceneDescriptor scene)
{
    for(uint32_t i = 0; i < scene.istream_pool_size; i++)
    {
        oct_print_atom_index_stream(oct, scene.istream_pool[i]);
    }
}


void oct_free_scene_descriptor(oct_sceneDescriptor* const scene)
{
    if(scene != NULL)
    {
        free(scene->istream_pool);

        *scene = (oct_sceneDescriptor){0};
    }
}
