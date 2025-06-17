#pragma once

#include "octane/oct.h"
#include "octane/oct_atoms.h"
#include <stdint.h>

typedef struct oct_rawDataDescriptor    oct_rawDataDescriptor;
typedef struct oct_sceneTreeNodePool    oct_sceneTreeNodePool;

/* Serialization of an oct files data */
struct oct_rawDataDescriptor
{
    /* VertexBufferPool */
    
    uint32_t vbuf_pool_size;

    oct_vertexBufferAtom* vbuf_pool;


    /* IndexBufferPool */

    uint32_t ibuf_pool_size;

    oct_indexBufferAtom* ibuf_pool;


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


oct_rawDataDescriptor oct_parse_raw_data_descriptor(oct_file oct);

void oct_free_raw_data_descriptor(oct_rawDataDescriptor* const scene);
