#pragma once
#include "data/dbuf.h"
#include "octane/oct/atoms.h"
#include "octane/oct/oct.h"
#include <stdint.h>

typedef struct oct_sceneDescriptor  oct_sceneDescriptor;

/* Serialization of an oct files data.
 * WARN: This *references* `oct_file` fields, do not free `oct_file` until you're done with this. */
struct oct_sceneDescriptor
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

    /* SceneTreeNodePool */

    uint32_t scene_tree_pool_size;

    oct_sceneTreeNodeAtom* scene_tree_node_pool;

    /* == EXTERNAL VALUES == */

    dbuf ibuf_file;

    uint32_t ibuf_stride;

    dbuf vbuf_file;
};

oct_sceneDescriptor oct_parse_scene_tree(oct_file oct);

void oct_free_scene_descriptor(oct_sceneDescriptor* const scene);
