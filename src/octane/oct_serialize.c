#include "oct_internal.h"
#include "octane/oct.h"
#include "octane/oct_atoms.h"
#include "octane/oct_scene.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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

    if(name_key == _oct_ant.vstream_element_name_color_add)
        return OCT_VSTREAM_ELEMENT_NAME_COLOR_ADD;

    if(name_key == _oct_ant.vstream_element_name_color_occl)
        return OCT_VSTREAM_ELEMENT_NAME_COLOR_OCC;

    if(name_key == _oct_ant.vstream_element_name_vertex_baked)
        return OCT_VSTREAM_ELEMENT_NAME_VERTEX_BAKED;

    return OCT_VSTREAM_ELEMENT_NAME_NONE;
}

static oct_sceneNodeType _parse_scene_node_type_from_name(uint16_t name_key)
{
    if(name_key == _oct_ant.scene_node_type_scene)
        return OCT_SCENE_NODE_TYPE_SCENE;

    if(name_key == _oct_ant.scene_node_type_transform)
        return OCT_SCENE_NODE_TYPE_TRANSFORM;

    if(name_key == _oct_ant.scene_node_type_geometry)
        return OCT_SCENE_NODE_TYPE_GEOMETRY;

    if(name_key == _oct_ant.scene_node_type_sub_geometry)
        return OCT_SCENE_NODE_TYPE_SUB_GEOMETRY;

    if(name_key == _oct_ant.scene_node_type_sub_geometry_lit)
        return OCT_SCENE_NODE_TYPE_SUB_GEOMETRY_LIT;

    if(name_key == _oct_ant.scene_node_type_camera)
        return OCT_SCENE_NODE_TYPE_CAMERA;

    if(name_key == _oct_ant.scene_node_type_light)
        return OCT_SCENE_NODE_TYPE_LIGHT;

    return OCT_SCENE_NODE_TYPE_NONE;
}

static oct_sceneLightType _parse_scene_light_type_from_name(uint16_t name_key)
{
    if(name_key == _oct_ant.scene_node_light_type_directional)
        return OCT_SCENE_LIGHT_TYPE_DIRECTIONAL;

    if(name_key == _oct_ant.scene_node_light_type_point)
        return OCT_SCENE_LIGHT_TYPE_POINT;

    if(name_key == _oct_ant.scene_node_light_type_ambient)
        return OCT_SCENE_LIGHT_TYPE_AMBIENT;

    return OCT_SCENE_LIGHT_TYPE_NONE;
}


oct_indexStreamAtom _oct_atom_read_index_stream(oct_file oct, uint32_t start_idx, uint32_t end_idx)
{
    oct_indexStreamAtom atom = (oct_indexStreamAtom){0};

    atom.atom_id = oct.data_tree[start_idx].name_key;

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

    atom.atom_id = oct.data_tree[start_idx].name_key;

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
                atom.element_flags |= atom.elements[j].attribute_type;
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

    atom.atom_id = oct.data_tree[start_idx].name_key;

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

oct_sceneTreeNodeAtom _oct_atom_read_scene_tree_node(oct_file oct, uint32_t start_idx, uint32_t end_idx)
{
    oct_sceneTreeNodeAtom atom = (oct_sceneTreeNodeAtom){0};

    atom.atom_id = oct.data_tree[start_idx].name_key;

    for(uint32_t i = start_idx+1; i < end_idx; i++)
    {
        oct_atomNode node = oct.data_tree[i];

        if(node.st_key == _oct_ant.scene_node_name)
        {
            atom.node_name = *(uint16_t*)node.data;
        }
        else if(node.st_key == _oct_ant.type)
        {
            uint16_t raw = *(uint16_t*)node.data;
            atom.type = _parse_scene_node_type_from_name(raw);
        }
        else if(node.st_key == _oct_ant.scene_node_parent_node_refs)
        {
            if(node.elem_count > 1)
                fprintf(stderr, "Element count more than 1!\n");

            atom.parent_node_refs = *(uint32_t*)node.data;
        }
        else if(node.st_key == _oct_ant.scene_node_local_to_parent_matrix)
        {
            atom.local_to_parent_matrix = *(mat4x4f*)node.data;
        }
        else if(node.st_key == _oct_ant.scene_node_visible)
        {
            atom.geometry.visible = *(bool*)node.data;
        }
        else if(node.st_key == _oct_ant.scene_node_mesh_name)
        {
            atom.geometry.mesh_name = *(uint16_t*)node.data;
        }
        else if(node.st_key == _oct_ant.scene_node_material_ref)
        {
            atom.sub_geometry.material_reference = *(uint32_t*)node.data;
        }
        else if(node.st_key == _oct_ant.scene_node_vstream_refs)
        {
            atom.sub_geometry.vstream_count = node.elem_count;
            atom.sub_geometry.vstream_refs = (uint32_t*)node.data;
        }
        else if(node.st_key == _oct_ant.scene_node_istream_ref)
        {
            atom.sub_geometry.istream_ref = *(uint32_t*)node.data;
        }
        else if(node.st_key == _oct_ant.scene_node_is_orthographic)
        {
            atom.camera.is_orthographic = *(bool*)node.data;
        }
        else if(node.st_key == _oct_ant.scene_node_focal_length)
        {
            atom.camera.focal_length = *(float*)node.data;
        }
        else if(node.st_key == _oct_ant.scene_node_camera_scale)
        {
            atom.camera.scale = *(float*)node.data;
        }
        else if(node.st_key == _oct_ant.scene_node_near_clip)
        {
            atom.camera.near_clip = *(float*)node.data;
        }
        else if(node.st_key == _oct_ant.scene_node_far_clip)
        {
            atom.camera.far_clip = *(float*)node.data;
        }
        else if(node.st_key == _oct_ant.scene_node_horizontal_film_aperture)
        {
            atom.camera.horizontal_film_aperture = *(float*)node.data;
        }
        else if(node.st_key == _oct_ant.scene_node_vertical_film_aperture)
        {
            atom.camera.vertical_film_aperture = *(float*)node.data;
        }
        else if(node.st_key == _oct_ant.scene_node_lens_squeeze_ratio)
        {
            atom.camera.lens_squeeze_ratio = *(float*)node.data;
        }
        else if(node.st_key == _oct_ant.scene_node_light_type)
        {
            uint16_t raw = *(uint16_t*)node.data;
            atom.light.type = _parse_scene_light_type_from_name(raw);
        }
        else if(node.st_key == _oct_ant.scene_node_light_color)
        {
            atom.light.light_color = *(vec3f*)node.data;
        }
        else if(node.st_key == _oct_ant.scene_node_light_intensity)
        {
            atom.light.light_intensity = *(float*)node.data;
        }
        else if(node.st_key == _oct_ant.scene_node_ambient_shade)
        {
            atom.light.ambient_shade = *(float*)node.data;
        }
        else if(node.st_key == _oct_ant.scene_node_shadow_color)
        {
            atom.light.shadow_color = *(vec3f*)node.data;
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

void _oct_parse_scene_tree_node_pool(oct_sceneDescriptor* const scene, oct_file oct)
{
    uint32_t* stream_table = _load_cache_hierarchy_named(
        _oct_ant.scene_tree_node_pool, _oct_ant.scene_node, 
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

oct_sceneDescriptor oct_parse_scene_descriptor(oct_file oct)
{
    oct_sceneDescriptor scene = (oct_sceneDescriptor){0};

    oct_init_atom_name_table(oct);

    _oct_parse_index_stream_pool(&scene, oct);

    _oct_parse_vertex_stream_pool(&scene, oct);

    _oct_parse_scene_tree_node_pool(&scene, oct);

    return scene;
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

        free(scene->scene_tree_node_pool);

        *scene = (oct_sceneDescriptor){0};
    }
}
