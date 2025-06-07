#include "cglm/mat4.h"
#include "ocelot/engine/scene.h"
#include "ocelot/engine/model.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

ocl_sceneMesh* ocl_find_mesh(ocl_scene scene, uint32_t mesh_id)
{
    if(scene.mesh_table == NULL)
        return NULL;

    for(uint32_t i = 0; i < scene.mesh_count; i++)
    {
        ocl_sceneMesh* mesh = &scene.mesh_table[i];
        if(mesh->id == mesh_id)
            return mesh;
    }

    return NULL;
}

void ocl_link_child(ocl_scene* const scene, uint32_t parent_idx, uint32_t child_idx)
{
    ocl_sceneNode* parent = &scene->node_table[parent_idx];
    ocl_sceneNode* child = &scene->node_table[child_idx];

    //Set the parent for the child
    child->parent = parent;

    if(parent->children == NULL)
    {
        //Allocate a new child array for the parent if it doesnt exist
        parent->children = calloc(1, sizeof(ocl_sceneNode*));
    }
    else
    {
        //Allocate a new space for the child if the child array exists
        parent->children = realloc(parent->children, (parent->child_count+1) * sizeof(ocl_sceneNode*));
    }

    //Add the child to the parent child array
    parent->children[parent->child_count] = child;
    parent->child_count++;

    glm_mat4_mul(
        child->local_to_parent_matrix,
        parent->world_to_parent_matrix,
        child->world_to_parent_matrix
    );
}


void ocl_unload_scene(ocl_scene* const scene)
{
    printf("Unloading scene\n");
    for(uint32_t i = 0; i < scene->node_count; i++)
    {
        ocl_sceneNode* node = &scene->node_table[i];
        free(node->node_name);
        free(node->children);
    }
    free(scene->node_table);

    for(uint32_t i = 0; i < scene->object_count; i++)
    {
        ocl_sceneObject* obj = &scene->object_table[i];
        free(obj->mesh_list);
        free(obj->mesh_name);
    }
    free(scene->object_table);

    for(uint32_t i = 0; i < scene->mesh_count; i++)
    {
        ocl_sceneMesh* mesh = &scene->mesh_table[i];

        free(mesh->vstream_refs);
        oce_free_model(&mesh->model);
    }
    free(scene->mesh_table);

    *scene = (ocl_scene){0};
}
