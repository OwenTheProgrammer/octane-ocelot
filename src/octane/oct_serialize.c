#include "oct_scene_internal.h"
#include "octane/oct.h"
#include "octane/oct_atoms.h"
#include <stdint.h>
#include <stdlib.h>

static uint32_t* _load_cache_hierarchy_indexed(uint32_t start_idx, uint16_t target_node, uint32_t* const node_count, oct_file oct)
{
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

    *node_count = count;

    //Add the read ending as a delim for the last node
    node_cache[count++] = start_idx;
    node_cache = realloc(node_cache, count * sizeof(uint32_t));

    return node_cache;
}


static uint32_t* _load_cache_hierarchy_named(uint16_t root_node, uint16_t target_node, uint32_t* const node_count, oct_file oct)
{
    //Find the root node
    uint32_t pool_index = oct_get_atom_node_index(oct, root_node);
    return _load_cache_hierarchy_indexed(pool_index, target_node, node_count, oct);
}

static oct_vstreamElementName _parse_element_type_from_name(uint16_t name_key)
{
    if(name_key == _oct_ant.vstream_element_name_position)
        return OCT_VSTREAM_ELEMENT_NAME_POSITION;

    if(name_key == _oct_ant.vstream_element_name_uv1)
        return OCT_VSTREAM_ELEMENT_NAME_UV1;

    if(name_key == _oct_ant.vstream_element_name_normal)
        return OCT_VSTREAM_ELEMENT_NAME_NORMAL;

    if(name_key == _oct_ant.vstream_element_name_tangent)
        return OCT_VSTREAM_ELEMENT_NAME_TANGENT;

    if(name_key == _oct_ant.vstream_element_name_binormal)
        return OCT_VSTREAM_ELEMENT_NAME_BINORMAL;

    if(name_key == _oct_ant.vstream_element_name_lightmapUV)
        return OCT_VSTREAM_ELEMENT_NAME_LIGHTMAP_UV;

    return OCT_VSTREAM_ELEMENT_NAME_NONE;
}


oct_indexStreamAtom _oct_atom_read_index_stream(oct_file oct, uint32_t start_idx, uint32_t end_idx)
{
    oct_indexStreamAtom atom = (oct_indexStreamAtom){0};

    oct_atomNode start_node = oct.data_tree[start_idx];
    uint32_t start_level = start_node.header.tree_level;

    atom.name_key = start_node.name_key;

    for(uint32_t i = start_idx+1; i < end_idx; i++)
    {
        oct_atomNode node = oct.data_tree[i];

        // TODO: Stream primitive is not read
        // TODO: No checks for endian type currently, although not needed

        if(node.st_key == _oct_ant.length)
        {
            atom.length = *(uint32_t*)node.data;
        }
        else if(node.st_key == _oct_ant.index_buffer_reference)
        {
            atom.buffer_reference = *(uint32_t*)node.data;
        }
        else if(node.st_key == _oct_ant.index_buffer_offset)
        {
            atom.buffer_offset = *(uint32_t*)node.data;
        }
        else if(node.st_key == _oct_ant.index_stream_primitive)
        {
            atom.stream_primitive = OCT_PRIMITIVE_TYPE_TRIANGLES;
        }

    }

    return atom;
}


oct_vertexStreamAtom _oct_atom_read_vertex_stream(oct_file oct, uint32_t start_idx, uint32_t end_idx)
{
    oct_vertexStreamAtom atom = (oct_vertexStreamAtom){0};

    oct_atomNode start_node = oct.data_tree[start_idx];
    uint32_t start_level = start_node.header.tree_level;

    atom.node_name = start_node.name_key;

    for(uint32_t i = start_idx+1; i < end_idx; i++)
    {
        oct_atomNode node = oct.data_tree[i];

        if(node.st_key == _oct_ant.length)
        {
            atom.length = *(uint32_t*)node.data;
        }
        else if(node.st_key == _oct_ant.width)
        {
            atom.width = *(uint32_t*)node.data;
        }
        else if(node.st_key == _oct_ant.vstream_buffer_reference)
        {
            atom.buffer_reference = *(uint32_t*)node.data;
        }
        else if(node.st_key == _oct_ant.vstream_buffer_offset)
        {
            atom.buffer_offset = *(uint32_t*)node.data;
        }
        else if(node.st_key == _oct_ant.vstream_elements)
        {
            uint32_t* stream_table = _load_cache_hierarchy_indexed(
                i, _oct_ant.vstream_element, &atom.element_count, oct
            );

            atom.elements = calloc(atom.element_count, sizeof(oct_vstreamElementAtom));

            for(uint32_t j = 0; j < atom.element_count; j++)
            {
                atom.elements[j] = _oct_atom_read_vstream_element(oct, stream_table[j], stream_table[j+1]);
                atom.element_flags |= atom.elements[j].type;
            }

            //Skip iterator to the ending of the array
            i = stream_table[atom.element_count];
            free(stream_table);
        }
    }

    return atom;
}


oct_vstreamElementAtom _oct_atom_read_vstream_element(oct_file oct, uint32_t start_idx, uint32_t end_idx)
{
    oct_vstreamElementAtom atom = (oct_vstreamElementAtom){0};

    oct_atomNode start_node = oct.data_tree[start_idx];
    uint32_t start_level = start_node.header.tree_level;

    atom.node_name = start_node.name_key;

    for(uint32_t i = start_idx+1; i < end_idx; i++)
    {
        oct_atomNode node = oct.data_tree[i];

        if(node.st_key == _oct_ant.type)
        {
            uint32_t raw = *(uint32_t*)node.data;
            atom.type = (oct_elementType)raw;
        }
        else if(node.st_key == _oct_ant.name)
        {
            atom.name = *(uint16_t*)node.data;
            atom.attribute_type = _parse_element_type_from_name(atom.name);
        }
        else if(node.st_key == _oct_ant.offset)
        {
            atom.offset = *(uint32_t*)node.data;
        }
    }

    return atom;
}


void _oct_parse_index_stream_pool(oct_sceneDescriptor* const scene, oct_file oct)
{
    //Load the hierarchy of the IndexStreamPool
    uint32_t* stream_table = _load_cache_hierarchy_named(
        _oct_ant.index_stream_pool, _oct_ant.index_stream,
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


void _oct_parse_vertex_stream_pool(oct_sceneDescriptor* const scene, oct_file oct)
{
    //Load the hierarchy of the VertexStreamPool
    uint32_t* stream_table = _load_cache_hierarchy_named(
        _oct_ant.vertex_stream_pool, _oct_ant.vertex_stream,
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
