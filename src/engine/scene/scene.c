#include "engine/core/scene.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void oce_link_child_to_parent(oce_scene* const scene, uint32_t parent_idx, uint32_t child_idx)
{
    oce_sceneNode* parent = &scene->node_table[parent_idx];
    oce_sceneNode* child = &scene->node_table[child_idx];

    //Set the childs parent reference
    child->parent = parent;

    if(parent->child_count == 0 || parent->children == NULL)
    {
        //Allocate the children table for the parent
        parent->children = calloc(1, sizeof(oce_sceneNode*));
    }
    else
    {
        //Allocate new space for the child
        parent->children = realloc(parent->children, (parent->child_count+1) * sizeof(oce_sceneNode*));
    }

    //Add the child to the parents children list
    parent->children[parent->child_count++] = child;
}

oce_model* oce_find_model(oce_scene* const scene, uint32_t buffer_count, uint32_t* const buffer_list)
{
    oce_model* ptr = NULL;

    for(uint32_t i = 0; i < scene->model_count && ptr == NULL; i++)
    {
        oce_model model = scene->model_table[i];

        // Skip if the stream counts arent the same
        if(model.stream_count != buffer_count)
            continue;

        // NOTE: This is prone to reorded arrays falsely being skipped
        // Test the stream list to being the exact same
        if( memcmp(buffer_list, model.stream_table, model.stream_count * sizeof(uint32_t)) == 0 )
            ptr = &scene->model_table[i];
    }

    return ptr;
}
