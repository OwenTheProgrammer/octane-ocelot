#include "cglm/mat4.h"
#include "engine/core/scene.h"
#include "engine/graphics/model.h"
#include "io/filepath.h"
#include "ocelot/scene/data.h"
#include "octane/ibuf/ibuf.h"
#include "octane/oct/atoms.h"
#include "octane/oct/enums.h"
#include "octane/oct/oct.h"
#include "engine/internal.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static char* _copy_oct_name(oce_scene scene, uint16_t string_idx)
{
    oct_file* oct = scene.oct_data.file_data;
    oct_string ref = oct->string_table[string_idx];

    char* str = calloc(ref.len+1, sizeof(char));
    strncpy(str, ref.data, ref.len);

    return str;
}

static void _common_node_init(oce_scene* const scene, oct_sceneTreeNodeAtom atom)
{
    oce_sceneNode* node = &scene->node_table[scene->node_count];

    //Copy the nodes name
    node->name = _copy_oct_name(*scene, atom.node_name);

    //Read the `LocalToParentMatrix`
    glm_mat4_make(atom.local_to_parent_matrix, node->local_to_parent_matrix);

    //Link the child to its parent reference
    oce_link_child_to_parent(scene, atom.parent_node_refs, scene->node_count);
}

static void _load_scene_node(oce_scene* const scene)
{
    oce_sceneNode* node = &scene->node_table[scene->node_count];

    filepath_t world_path = scene->oct_data.file_data->world_path;
    filepath_t world_name = fs_path_top_filename(world_path);

    node->name = calloc(world_name.length+1, sizeof(char));
    strncpy(node->name, world_name.path, world_name.length);

    glm_mat4_identity(node->local_to_parent_matrix);

    scene->node_count++;
}

static void _load_transform_node(oce_scene* const scene, oct_sceneTreeNodeAtom atom)
{
    oce_sceneNode* node = &scene->node_table[scene->node_count];

    _common_node_init(scene, atom);

    scene->node_count++;
}

static void _load_geometry_node(oce_scene* const scene, oct_sceneTreeNodeAtom atom)
{
    oce_sceneNode* node = &scene->node_table[scene->node_count];

    _common_node_init(scene, atom);

    node->mesh_name = _copy_oct_name(*scene, atom.geometry.mesh_name);

    scene->node_count++;
}



static void _load_subgeometry_node(oce_scene* const scene, oct_sceneTreeNodeAtom atom)
{
    oce_sceneNode* node = &scene->node_table[scene->node_count];

    _common_node_init(scene, atom);

    oct_sceneSubGeometryNode geom = atom.sub_geometry;

    uint32_t stream_count = geom.vstream_count + 1;
    uint32_t* stream_refs = calloc(stream_count, sizeof(uint32_t));

    stream_refs[0] = geom.istream_ref;
    memcpy(&stream_refs[1], geom.vstream_refs, geom.vstream_count * sizeof(uint32_t));

    oce_model* model = oce_find_model(scene, stream_count, stream_refs);

    if(model == NULL)
    {
        //We havent loaded the model yet
        model = &scene->model_table[scene->model_count];

        // Set the stream data
        model->stream_count = stream_count;
        model->stream_table = stream_refs;

        // Load the model from the streams
        oce_model_load_streams(model, &scene->oct_data);

        // Increment the amount of models
        scene->model_count++;
    }

    node->mesh_ptr = model;

    scene->node_count++;
}


oce_scene oce_load_scene(ocl_sceneData* const data)
{
    oce_scene scene = (oce_scene){0};

    scene.oct_data = *data;

    oct_sceneDescriptor* desc = data->scene_descriptor;

    //Load the scene tree
    scene.node_table = calloc(desc->scene_tree_pool_size, sizeof(oce_sceneNode));
    scene.model_table = calloc(desc->scene_tree_pool_size, sizeof(oce_model));

    for(uint32_t i = 0; i < desc->scene_tree_pool_size; i++)
    {
        oct_sceneTreeNodeAtom atom = desc->scene_tree_node_pool[i];

        switch(atom.type)
        {
            case OCT_SCENE_NODE_TYPE_SCENE:
                _load_scene_node(&scene);
                break;

            case OCT_SCENE_NODE_TYPE_TRANSFORM:
                _load_transform_node(&scene, atom);
                break;

            case OCT_SCENE_NODE_TYPE_GEOMETRY:
                _load_geometry_node(&scene, atom);
                break;

            case OCT_SCENE_NODE_TYPE_SUB_GEOMETRY:
            case OCT_SCENE_NODE_TYPE_SUB_GEOMETRY_LIT:
                _load_subgeometry_node(&scene, atom);
                break;

            default:
                scene.node_count++;
                break;
        }
    }

    scene.node_table = realloc(scene.node_table, scene.node_count * sizeof(oce_sceneNode));
    scene.model_table = realloc(scene.model_table, scene.model_count * sizeof(oce_model));

    LOG_INFO("Loaded %u nodes %u models\n", scene.node_count, scene.model_count);
    //printf("[engine]: Loaded %u nodes %u models\n", scene.node_count, scene.model_count);

    return scene;
}


void oce_free_scene_node(oce_sceneNode* const node)
{
    if(node != NULL)
    {
        free(node->name);
        free(node->children);
        free(node->mesh_name);
    }
}


void oce_free_scene(oce_scene* const scene)
{
    if(scene != NULL)
    {
        for(uint32_t i = 0; i < scene->node_count; i++)
        {
            oce_free_scene_node(&scene->node_table[i]);
        }
        free(scene->node_table);

        for(uint32_t i = 0; i < scene->model_count; i++)
        {
            oce_free_model(&scene->model_table[i]);
        }
        free(scene->model_table);
    }
}
