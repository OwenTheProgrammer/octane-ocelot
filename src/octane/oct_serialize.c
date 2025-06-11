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

/* Check if `name_key` matches the [S]cene [N]ode [T]ype*/
#define EVAL_SNT(NODE_TYPE, ELEM_TYPE) \
    if(name_key == _oct_aet.SceneTreeNodePool.Node.Type.NODE_TYPE)  \
        return OCT_SCENE_NODE_TYPE_##ELEM_TYPE;

/* Check if `name_key` matches the [S]cene [L]ight [T]ype */
#define EVAL_SLT(NODE_TYPE, ELEM_TYPE) \
    if(name_key == _oct_aet.SceneTreeNodePool.Node.LightType.NODE_TYPE) \
        return OCT_SCENE_LIGHT_TYPE_##ELEM_TYPE;

/* Check if `name_key` matches the [V]stream [E]lement [N]ame */
#define EVAL_VEN(OCT_TYPE, ELEM_TYPE) \
    if(name_key == _oct_aet.VertexStreamPool.VertexStream.Elements.Element.Name.OCT_TYPE)  \
        return OCT_VSTREAM_ELEMENT_NAME_##ELEM_TYPE;

/* Check if `name_key` matches the [P]rimitive [T]ype */
#define EVAL_PT(OCT_TYPE, ELEM_TYPE) \
    if(name_key == _oct_aet.IndexStreamPool.IndexStream.IndexStreamPrimitive.OCT_TYPE)  \
        return OCT_PRIMITIVE_TYPE_##ELEM_TYPE;

static oct_sceneNodeType _scene_node_type_from_st_idx(uint16_t name_key)
{
    EVAL_SNT(Scene, SCENE)
    EVAL_SNT(Transform, TRANSFORM)
    EVAL_SNT(Geometry, GEOMETRY)
    EVAL_SNT(SubGeometry, SUB_GEOMETRY)
    EVAL_SNT(SubGeometryLit, SUB_GEOMETRY_LIT)
    EVAL_SNT(Camera, CAMERA)
    EVAL_SNT(Light, LIGHT)

    return OCT_SCENE_NODE_TYPE_NONE;
}

static oct_sceneLightType _scene_light_type_from_st_idx(uint16_t name_key)
{
    EVAL_SLT(Directional, DIRECTIONAL)
    EVAL_SLT(Point, POINT)
    EVAL_SLT(Ambient, AMBIENT)

    return OCT_SCENE_LIGHT_TYPE_NONE;
}

static oct_vstreamElementName _vstream_element_name_from_st_idx(uint16_t name_key)
{
    EVAL_VEN(Position, POSITION)
    EVAL_VEN(Uv1, UV1)
    EVAL_VEN(Tangent, TANGENT)
    EVAL_VEN(Normal, NORMAL)
    EVAL_VEN(Binormal, BINORMAL)
    EVAL_VEN(lightmapUV, LIGHTMAP_UV)
    EVAL_VEN(ColorAdditive, COLOR_ADD)
    EVAL_VEN(ColorOcclusion, COLOR_OCC)
    EVAL_VEN(VertexBaked, VERTEX_BAKED)

    return OCT_VSTREAM_ELEMENT_NAME_NONE;
}

static oct_primitiveType _primitive_type_from_st_idx(uint16_t name_key)
{
    EVAL_PT(TRIANGLES, TRIANGLES)

    return OCT_PRIMITIVE_TYPE_NONE;
}

#undef EVAL_SNT
#undef EVAL_SLT
#undef EVAL_VEN
#undef EVAL_PT

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
static uint32_t* _load_cache_hierarchy_indexed(uint32_t start_idx, uint16_t target_node, uint32_t* const node_count, oct_file oct)
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


static uint32_t* _load_cache_hierarchy_named(uint16_t root_node, uint16_t target_node, uint32_t* const node_count, oct_file oct)
{
    //Find the root node
    uint32_t pool_index = _find_index_of_node(oct, root_node);
    return _load_cache_hierarchy_indexed(pool_index, target_node, node_count, oct);
}

/* Reads an instance of IndexStreamPool/IndexStream from `start_idx` to `end_idx` */
oct_indexStreamAtom _oct_atom_read_index_stream(oct_file oct, uint32_t start_idx, uint32_t end_idx)
{
    oct_indexStreamAtom atom = (oct_indexStreamAtom){0};

    atom.atom_id = oct.data_tree[start_idx].name_key;

    for(uint32_t i = start_idx+1; i < end_idx; i++)
    {
        oct_atomNode node = oct.data_tree[i];

        if(node.st_key == _oct_ant.IndexStreamPool.IndexStream.Length)
        {
            atom.length = *(uint32_t*)node.data;
        }
        else if(node.st_key == _oct_ant.IndexStreamPool.IndexStream.IndexBufferReference)
        {
            atom.buffer_reference = *(uint32_t*)node.data;
        }
        else if(node.st_key == _oct_ant.IndexStreamPool.IndexStream.IndexBufferOffset)
        {
            atom.buffer_offset = *(uint32_t*)node.data;
        }
        else if(node.st_key == _oct_ant.IndexStreamPool.IndexStream.IndexStreamPrimitive)
        {
            uint16_t key = *(uint16_t*)node.data;
            atom.stream_primitive = _primitive_type_from_st_idx(key);
        }
    }

    return atom;
}

static uint32_t _atom_load_elements_table(uint32_t start_idx, oct_vertexStreamAtom* const atom_dst, oct_file oct)
{
    //Find all the nodes with type "Element" in "VertexStreamPool/VertexStream/Elements"
    uint32_t* stream_table = _load_cache_hierarchy_indexed(
        start_idx,
        _oct_ant.VertexStreamPool.VertexStream.Elements.Element._header,
        &atom_dst->element_count,
        oct
    );

    //Allocate space for the Element nodes
    atom_dst->elements = calloc(atom_dst->element_count, sizeof(oct_vertexStreamAtom));

    //Load each Element node
    for(uint32_t i = 0; i < atom_dst->element_count; i++)
    {
        atom_dst->elements[i] = _oct_atom_read_vstream_element(oct, stream_table[i], stream_table[i+1]);
        atom_dst->element_flags |= atom_dst->elements[i].attribute_type;
    }

    //Return the pointer to the end of the tree
    uint32_t iter = stream_table[atom_dst->element_count];
    free(stream_table);

    return iter;
}

/* Reads an instance of VertexStreamPool/VertexStream from `start_idx` to `end_idx` */
oct_vertexStreamAtom _oct_atom_read_vertex_stream(oct_file oct, uint32_t start_idx, uint32_t end_idx)
{
    oct_vertexStreamAtom atom = (oct_vertexStreamAtom){0};

    atom.atom_id = oct.data_tree[start_idx].name_key;

    for(uint32_t i = start_idx+1; i < end_idx; i++)
    {
        oct_atomNode node = oct.data_tree[i];

        if(node.st_key == _oct_ant.VertexStreamPool.VertexStream.Length)
        {
            atom.length = *(uint32_t*)node.data;
        }
        else if(node.st_key == _oct_ant.VertexStreamPool.VertexStream.Width)
        {
            atom.width = *(uint32_t*)node.data;
        }
        else if(node.st_key == _oct_ant.VertexStreamPool.VertexStream.VertexBufferReference)
        {
            atom.buffer_reference = *(uint32_t*)node.data;
        }
        else if(node.st_key == _oct_ant.VertexStreamPool.VertexStream.VertexBufferOffset)
        {
            atom.buffer_offset = *(uint32_t*)node.data;
        }
        else if(node.st_key == _oct_ant.VertexStreamPool.VertexStream.Elements._header)
        {
            i = _atom_load_elements_table(i, &atom, oct);
        }
    }

    return atom;
}

/* Reads an instance of VertexStreamPool/VertexStream/Elements/Element from `start_idx` to `end_idx` */
oct_vstreamElementAtom _oct_atom_read_vstream_element(oct_file oct, uint32_t start_idx, uint32_t end_idx)
{
    oct_vstreamElementAtom atom = (oct_vstreamElementAtom){0};

    atom.atom_id = oct.data_tree[start_idx].name_key;

    for(uint32_t i = start_idx+1; i < end_idx; i++)
    {
        oct_atomNode node = oct.data_tree[i];

        if(node.st_key == _oct_ant.VertexStreamPool.VertexStream.Elements.Element.Type)
        {
            uint32_t raw = *(uint32_t*)node.data;
            atom.type = (oct_elementType)raw;
        }
        else if(node.st_key == _oct_ant.VertexStreamPool.VertexStream.Elements.Element.Name)
        {
            atom.name = *(uint16_t*)node.data;
            atom.attribute_type = _vstream_element_name_from_st_idx(atom.name);
        }
        else if(node.st_key == _oct_ant.VertexStreamPool.VertexStream.Elements.Element.Offset)
        {
            atom.offset = *(uint32_t*)node.data;
        }
    }

    return atom;
}

static inline bool _stn_handle_geometry(oct_sceneTreeNodeAtom* const atom, uint16_t st_key, oct_atomNode node)
{
    if(st_key == _oct_ant.SceneTreeNodePool.Node.Visible)
    {
        atom->geometry.visible = *(bool*)node.data;
        return true;
    }
    else if(st_key == _oct_ant.SceneTreeNodePool.Node.MeshName)
    {
        atom->geometry.mesh_name = *(uint16_t*)node.data;
        return true;
    }

    return false;
}

static inline bool _stn_handle_sub_geometry(oct_sceneTreeNodeAtom* const atom, uint16_t st_key, oct_atomNode node)
{
    if(st_key == _oct_ant.SceneTreeNodePool.Node.MaterialReference)
    {
        atom->sub_geometry.material_reference = *(uint32_t*)node.data;
        return true;
    }
    else if(st_key == _oct_ant.SceneTreeNodePool.Node.VertexStreamReferences)
    {
        atom->sub_geometry.vstream_count = node.elem_count;
        atom->sub_geometry.vstream_refs = (uint32_t*)node.data;
        return true;
    }
    else if(st_key == _oct_ant.SceneTreeNodePool.Node.IndexStreamReference)
    {
        atom->sub_geometry.istream_ref = *(uint32_t*)node.data;
        return true;
    }

    return false;
}

static inline bool _stn_handle_camera(oct_sceneTreeNodeAtom* const atom, uint16_t st_key, oct_atomNode node)
{
    if(st_key == _oct_ant.SceneTreeNodePool.Node.IsOrthographic)
    {
        atom->camera.is_orthographic = *(bool*)node.data;
        return true;
    }
    else if(st_key == _oct_ant.SceneTreeNodePool.Node.FocalLength)
    {
        atom->camera.focal_length = *(float*)node.data;
        return true;
    }
    else if(st_key == _oct_ant.SceneTreeNodePool.Node.CameraScale)
    {
        atom->camera.scale = *(float*)node.data;
        return true;
    }
    else if(st_key == _oct_ant.SceneTreeNodePool.Node.NearClipPlane)
    {
        atom->camera.near_clip = *(float*)node.data;
        return true;
    }
    else if(st_key == _oct_ant.SceneTreeNodePool.Node.FarClipPlane)
    {
        atom->camera.far_clip = *(float*)node.data;
        return true;
    }
    else if(st_key == _oct_ant.SceneTreeNodePool.Node.HorizontalFilmAperture)
    {
        atom->camera.horizontal_film_aperture = *(float*)node.data;
        return true;
    }
    else if(st_key == _oct_ant.SceneTreeNodePool.Node.VerticalFilmAperture)
    {
        atom->camera.vertical_film_aperture = *(float*)node.data;
        return true;
    }
    else if(st_key == _oct_ant.SceneTreeNodePool.Node.LensSqueezeRatio)
    {
        atom->camera.lens_squeeze_ratio = *(float*)node.data;
        return true;
    }

    return false;
}

static inline bool _stn_handle_light(oct_sceneTreeNodeAtom* const atom, uint16_t st_key, oct_atomNode node)
{
    if(st_key == _oct_ant.SceneTreeNodePool.Node.LightType)
    {
        uint16_t raw = *(uint16_t*)node.data;
        atom->light.type = _scene_light_type_from_st_idx(raw);
        return true;
    }
    else if(st_key == _oct_ant.SceneTreeNodePool.Node.LightColor)
    {
        if(node.elem_count != 3)
            fprintf(stderr, "Unhandled light color!\n");

        // NOTE: Copying to float[3] on stack, no allocations made
        memcpy(atom->light.light_color, node.data, sizeof(atom->light.light_color));
        return true;
    }
    else if(st_key == _oct_ant.SceneTreeNodePool.Node.LightIntensity)
    {
        atom->light.light_intensity = *(float*)node.data;
        return true;
    }
    else if(st_key == _oct_ant.SceneTreeNodePool.Node.AmbientShade)
    {
        atom->light.ambient_shade = *(float*)node.data;
        return true;
    }
    else if(st_key == _oct_ant.SceneTreeNodePool.Node.ShadowColor)
    {
        if(node.elem_count != 3)
            fprintf(stderr, "Unhandled shadow color!\n");

        // NOTE: Copying to float[3] on stack, no allocations made
        memcpy(atom->light.shadow_color, node.data, sizeof(atom->light.shadow_color));
        return true;
    }

    return false;
}

/* Reads and instance of type SceneTreeNodePool/Node from `start_idx` to `end_idx` */
oct_sceneTreeNodeAtom _oct_atom_read_scene_tree_node(oct_file oct, uint32_t start_idx, uint32_t end_idx)
{
    oct_sceneTreeNodeAtom atom = (oct_sceneTreeNodeAtom){0};

    atom.atom_id = oct.data_tree[start_idx].name_key;

    for(uint32_t i = start_idx+1; i < end_idx; i++)
    {
        oct_atomNode node = oct.data_tree[i];

        if(node.st_key == _oct_ant.SceneTreeNodePool.Node.NodeName)
        {
            atom.node_name = *(uint16_t*)node.data;
        }
        else if(node.st_key == _oct_ant.SceneTreeNodePool.Node.Type)
        {
            uint16_t raw = *(uint16_t*)node.data;
            atom.type = _scene_node_type_from_st_idx(raw);
        }
        else if(node.st_key == _oct_ant.SceneTreeNodePool.Node.ParentNodeReferences)
        {
            if(node.elem_count > 1)
                fprintf(stderr, "Element count more than 1!\n");

            atom.parent_node_refs = *(uint32_t*)node.data;
        }
        else if(node.st_key == _oct_ant.SceneTreeNodePool.Node.LocalToParentMatrix)
        {
            if(node.elem_count != 16)
                fprintf(stderr, "Matrix does not have 16 entries!\n");

            // NOTE: Copying to float[16], no memory allocations here
            memcpy(atom.local_to_parent_matrix, node.data, sizeof(atom.local_to_parent_matrix));
        }
        else
        {
            if(_stn_handle_geometry(&atom, node.st_key, node))
                continue;

            if(_stn_handle_sub_geometry(&atom, node.st_key, node))
                continue;

            if(_stn_handle_camera(&atom, node.st_key, node))
                continue;

            if(_stn_handle_light(&atom, node.st_key, node))
                continue;
        }
    }

    return atom;
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

    _oct_parse_index_stream_pool(&scene, oct);

    _oct_parse_vertex_stream_pool(&scene, oct);

    _oct_parse_scene_tree_node_pool(&scene, oct);

    return scene;
}


void oct_free_raw_data_descriptor(oct_rawDataDescriptor* const scene)
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

        *scene = (oct_rawDataDescriptor){0};
    }
}
