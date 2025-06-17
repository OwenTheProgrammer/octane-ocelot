#include "oct_internal.h"
#include "octane/oct.h"
#include "octane/oct_atoms.h"
#include "octane/oct_scene.h"
#include "octane/oct_nameBindings.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


/* Finds the index of the first node in the data tree with the type `key` */
static uint32_t _find_index_of_node(oct_file oct, uint16_t key)
{
    if(key == 0)
        return 0;

    for(uint32_t i = 0; i < oct.data_tree_length; i++)
    {
        if(oct.data_tree[i].st_key == key)
            return i;
    }

    return 0;
}

/* Preloads the target node type positions within an atom array */
uint32_t* _load_cache_hierarchy_indexed(uint32_t start_idx, uint16_t target_node, uint32_t* const node_count, oct_file oct)
{
    //The hierarchy level we want to stop at when we see again
    uint32_t pool_level = oct.data_tree[start_idx++].header.tree_level;

    //Allocate a table with much more than needed (max amount possible)
    uint32_t* node_cache = calloc(UINT16_MAX, sizeof(uint32_t));
    uint32_t count = 0;

    //Keep going until we read a node thats outside the root nodes level
    while(oct.data_tree[start_idx].header.tree_level > pool_level)
    {
        oct_atomNode node = oct.data_tree[start_idx];

        //Add the node to the cache if its the target
        if(node.st_key == target_node)
            node_cache[count++] = start_idx;

        start_idx++;
    }

    //Return the amount of target node occurances found
    *node_count = count;

    //Add the read ending as a delim for the last node
    node_cache[count++] = start_idx;
    node_cache = realloc(node_cache, count * sizeof(uint32_t));

    return node_cache;
}


uint32_t* _load_cache_hierarchy_named(uint16_t root_node, uint16_t target_node, uint32_t* const node_count, oct_file oct)
{
    //Find the root node
    uint32_t pool_index = _find_index_of_node(oct, root_node);
    return _load_cache_hierarchy_indexed(pool_index, target_node, node_count, oct);
}



/* Reads the entries of VertexBufferPool */
void _oct_parse_vertex_buffer_pool(oct_rawDataDescriptor* const scene, oct_file oct)
{
    //Load the hierarchy of the VertexBufferPool
    uint32_t* stream_table = _load_cache_hierarchy_named(
        _oct_ant.VertexBufferPool._header,
        _oct_ant.VertexBufferPool.VertexBuffer._header,
        &scene->vbuf_pool_size, oct
    );

    scene->vbuf_pool = calloc(scene->vbuf_pool_size, sizeof(oct_vertexBufferAtom));

    //Read the nodes at the cache positions
    for(uint32_t i = 0; i < scene->vbuf_pool_size; i++)
    {
        scene->vbuf_pool[i] = _oct_atom_read_vertex_buffer(oct, stream_table[i], stream_table[i+1]);
    }

    free(stream_table);
}

/* Reads the entries of IndexBufferPool */
void _oct_parse_index_buffer_pool(oct_rawDataDescriptor* const scene, oct_file oct)
{
    //Load the hierarchy of the IndexBufferPool
    uint32_t* stream_table = _load_cache_hierarchy_named(
        _oct_ant.IndexBufferPool._header,
        _oct_ant.IndexBufferPool.IndexBuffer._header,
        &scene->ibuf_pool_size, oct
    );

    scene->ibuf_pool = calloc(scene->ibuf_pool_size, sizeof(oct_indexBufferAtom));

    //Read the nodes at the cache positions
    for(uint32_t i = 0; i < scene->ibuf_pool_size; i++)
    {
        scene->ibuf_pool[i] = _oct_atom_read_index_buffer(oct, stream_table[i], stream_table[i+1]);
    }

    free(stream_table);
}

/* Reads the entries of IndexStreamPool */
void _oct_parse_index_stream_pool(oct_rawDataDescriptor* const scene, oct_file oct)
{
    //Load the hierarchy of the IndexStreamPool
    uint32_t* stream_table = _load_cache_hierarchy_named(
        _oct_ant.IndexStreamPool._header,
        _oct_ant.IndexStreamPool.IndexStream._header,
        &scene->istream_pool_size, oct
    );

    scene->istream_pool = calloc(scene->istream_pool_size, sizeof(oct_indexStreamAtom));

    // Read the nodes at the cache positions
    for(uint32_t i = 0; i < scene->istream_pool_size; i++)
    {
        scene->istream_pool[i] = _oct_atom_read_index_stream(oct, stream_table[i], stream_table[i+1]);
    }

    free(stream_table);
}


void _oct_parse_vertex_stream_pool(oct_rawDataDescriptor* const scene, oct_file oct)
{
    //Load the hierarchy of the VertexStreamPool
    uint32_t* stream_table = _load_cache_hierarchy_named(
        _oct_ant.VertexStreamPool._header,
        _oct_ant.VertexStreamPool.VertexStream._header,
        &scene->vstream_pool_size, oct
    );

    scene->vstream_pool = calloc(scene->vstream_pool_size, sizeof(oct_vertexStreamAtom));

    //Read the nodes at the cache positions
    for(uint32_t i = 0; i < scene->vstream_pool_size; i++)
    {
        scene->vstream_pool[i] = _oct_atom_read_vertex_stream(oct, stream_table[i], stream_table[i+1]);
    }

    free(stream_table);
}

void _oct_parse_scene_tree_node_pool(oct_rawDataDescriptor* const scene, oct_file oct)
{
    uint32_t* stream_table = _load_cache_hierarchy_named(
        _oct_ant.SceneTreeNodePool._header,
        _oct_ant.SceneTreeNodePool.Node._header,
        &scene->scene_tree_pool_size, oct
    );

    scene->scene_tree_node_pool = calloc(scene->scene_tree_pool_size, sizeof(oct_sceneTreeNodeAtom));

    //Read the nodes at the cache positions
    for(uint32_t i = 0; i < scene->scene_tree_pool_size; i++)
    {
        scene->scene_tree_node_pool[i] = _oct_atom_read_scene_tree_node(oct, stream_table[i], stream_table[i+1]);
    }

    free(stream_table);
}

oct_rawDataDescriptor oct_parse_raw_data_descriptor(oct_file oct)
{
    oct_rawDataDescriptor scene = (oct_rawDataDescriptor){0};

    oct_load_name_bindings(oct);

    _oct_parse_vertex_buffer_pool(&scene, oct);

    _oct_parse_index_buffer_pool(&scene, oct);

    _oct_parse_index_stream_pool(&scene, oct);

    _oct_parse_vertex_stream_pool(&scene, oct);

    _oct_parse_scene_tree_node_pool(&scene, oct);

    return scene;
}


void oct_free_raw_data_descriptor(oct_rawDataDescriptor* const scene)
{
    if(scene != NULL)
    {
        free(scene->vbuf_pool);

        free(scene->ibuf_pool);

        free(scene->istream_pool);

        for(uint32_t i = 0; i < scene->vstream_pool_size; i++)
        {
            free(scene->vstream_pool[i].elements);
        }
        free(scene->vstream_pool);

        free(scene->scene_tree_node_pool);

        *scene = (oct_rawDataDescriptor){0};
    }
}
