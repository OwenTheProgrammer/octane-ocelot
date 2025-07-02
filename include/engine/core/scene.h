#pragma once

#include "cglm/types.h"
#include "engine/graphics/model.h"
#include "ocelot/scene/data.h"
#include "octane/oct/scene_descriptor.h"

typedef struct oce_scene        oce_scene;
typedef struct oce_sceneNode    oce_sceneNode;

struct oce_sceneNode
{

    char* name;

    oce_sceneNode* parent;


    uint32_t child_count;

    oce_sceneNode** children;

    /* Represents a transformation relative to the parents coordinate space.
     * An identity matrix would represent sharing the parents orientation. */
    mat4 local_to_parent_matrix;

    char* mesh_name;

    oce_model* mesh_ptr;
};

struct oce_scene
{
    ocl_sceneData oct_data;

    oct_sceneDescriptor descriptor;


    uint32_t node_count;

    oce_sceneNode* node_table;


    uint32_t model_count;

    oce_model* model_table;

};

oce_scene oce_load_scene(ocl_sceneData* const data);

void oce_link_child_to_parent(oce_scene* const scene, uint32_t parent_idx, uint32_t child_idx);

oce_model* oce_find_model(oce_scene* const scene, uint32_t buffer_count, uint32_t* const buffer_list);

void oce_free_scene_node(oce_sceneNode* const node);

void oce_free_scene(oce_scene* const scene);
