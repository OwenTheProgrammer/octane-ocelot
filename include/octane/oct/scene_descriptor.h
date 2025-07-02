#pragma once
#include "octane/oct/atoms.h"
#include "octane/oct/oct.h"
#include <stdint.h>

typedef struct oct_sceneDescriptor  oct_sceneDescriptor;

/* Serialization of an oct files data.
 * WARN: This *references* `oct_file` fields, do not free `oct_file` until you're done with this. */
struct oct_sceneDescriptor
{
    /* VertexBufferPool */

    /* Amount of `VertexBuffer` atoms in the `VertexBufferPool` */
    uint32_t vbuf_pool_size;

    /* List of `VertexBufferPool/VertexBuffer` atoms */
    oct_vertexBufferAtom* vbuf_pool;

    /* IndexBufferPool */

    /* Amount of `IndexBuffer` atoms in the `IndexBufferPool` */
    uint32_t ibuf_pool_size;

    /* List of `IndexBufferPool/IndexBuffer` atoms */
    oct_indexBufferAtom* ibuf_pool;

    /* IndexStreamPool */

    /* Amount of `IndexStream` atoms in the `IndexStreamPool` */
    uint32_t istream_pool_size;

    /* List of `IndexStreamPool/IndexStream` atoms */
    oct_indexStreamAtom* istream_pool;

    /* VertexStreamPool */

    /* Amount of `VertexStream` atoms in the `VertexStreamPool` */
    uint32_t vstream_pool_size;

    /* List of `VertexStreamPool/VertexStream` atoms */
    oct_vertexStreamAtom* vstream_pool;

    /* SceneTreeNodePool */

    /* Amount of `Node` atoms in the `SceneTreeNodePool` */
    uint32_t scene_tree_pool_size;

    /* List of `SceneTreeNodePool/Node` atoms */
    oct_sceneTreeNodeAtom* scene_tree_node_pool;
};

oct_sceneDescriptor oct_parse_scene_tree(oct_file oct);

void oct_free_scene_descriptor(oct_sceneDescriptor* const scene);
