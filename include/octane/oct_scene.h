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

    /* VertexStreamPool */

    uint32_t vstream_pool_size;

    oct_vertexStreamAtom* vstream_pool;

    /* SceneTreeNodePool*/

    uint32_t scene_tree_pool_size;

    oct_sceneTreeNodeAtom* scene_tree_node_pool;
};


oct_sceneDescriptor oct_parse_scene_descriptor(oct_file oct);

void oct_print_scene_descriptor(oct_file oct, oct_sceneDescriptor scene);

void oct_free_scene_descriptor(oct_sceneDescriptor* const scene);
