#include "cglm/mat4.h"
#include "ocelot/engine/scene.h"
#include "octane/oct/oct.h"
#include "octane/oct/atoms.h"
#include "octane/oct/scene_descriptor.h"
#include "types/crc.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char* _copy_name(oct_file oct, uint16_t string_idx)
{
    oct_string ref = oct.string_table[string_idx];

    char* buf = calloc(ref.len+1, sizeof(char));
    strncpy(buf, ref.data, ref.len);

    return buf;
}

static ocl_sceneMesh _load_mesh(ocl_sceneObject* const obj_ref, bool lit, oct_sceneTreeNodeAtom atom)
{
    ocl_sceneMesh mesh = (ocl_sceneMesh){0};

    mesh.id = obj_ref->mesh_id;
    mesh.mesh_name = obj_ref->mesh_name;
    mesh.is_lit = lit;

    mesh.vstream_ref_count = atom.sub_geometry.vstream_count;
    mesh.vstream_refs = calloc(mesh.vstream_ref_count, sizeof(uint32_t));
    memcpy(mesh.vstream_refs, atom.sub_geometry.vstream_refs, mesh.vstream_ref_count * sizeof(uint32_t));

    mesh.istream_ref = atom.sub_geometry.istream_ref;

    //LOAD THE OCE_MODEL HERE
    printf("[%-14s]:\tLoading geometry %-48s\t[id: 0x%08X | iref: %u]\n",
           lit ? "SubGeometryLit" : "SubGeometry",
           mesh.mesh_name,
           mesh.id,
           mesh.istream_ref
    );

    return mesh;
}


static void _load_scene_atom(ocl_scene* const scene, oct_sceneTreeNodeAtom atom)
{
    //Construct the scene node
    ocl_sceneNode node = (ocl_sceneNode){0};

    node.type = OCL_SCENE_NODE_TYPE_SCENE;
    //uuid
    node.is_enabled = true;
    //oct name
    glm_mat4_identity(node.local_to_parent_matrix);
    glm_mat4_identity(node.world_to_parent_matrix);

    //Add the scene node to the node table
    scene->node_table[scene->node_count] = node;

    scene->node_count++;
}

static void _load_transform_atom(ocl_scene* const scene, oct_sceneTreeNodeAtom atom, oct_file oct)
{
    //Construct the scene node
    ocl_sceneNode node = (ocl_sceneNode){0};

    node.type = OCL_SCENE_NODE_TYPE_TRANSFORM;
    //uuid
    node.is_enabled = true;
    node.node_name = _copy_name(oct, atom.node_name);
    glm_mat4_make(atom.local_to_parent_matrix, node.local_to_parent_matrix);

    //Add the scene node to the node table
    scene->node_table[scene->node_count] = node;

    //Link the child
    ocl_link_child(scene, atom.parent_node_refs, scene->node_count);

    //Increment the counters
    scene->node_count++;
}

static void _load_geometry_atom(ocl_scene* const scene, oct_sceneTreeNodeAtom atom, oct_file oct)
{
    //Construct the scene node
    ocl_sceneNode node = (ocl_sceneNode){0};

    node.type = OCL_SCENE_NODE_TYPE_OBJECT;
    //uuid
    node.is_enabled = true;
    node.node_name = _copy_name(oct, atom.node_name);
    glm_mat4_make(atom.local_to_parent_matrix, node.local_to_parent_matrix);

    //Add the scene node to the node table
    scene->node_table[scene->node_count] = node;

    ocl_link_child(scene, atom.parent_node_refs, scene->node_count);

    //Create the scene object
    ocl_sceneObject object = (ocl_sceneObject){0};

    object.is_visible = atom.geometry.visible;
    object.mesh_name = _copy_name(oct, atom.geometry.mesh_name);
    object.mesh_id = crc32_from_string(object.mesh_name);

    //Add the object to the object table
    scene->object_table[scene->object_count] = object;

    //Link the Geometry object reference to the new object
    scene->node_table[scene->node_count].object_ref = &scene->object_table[scene->object_count];

    //Increment the counters
    scene->node_count++;
    scene->object_count++;
}

static void _load_sub_geometry_atom(ocl_scene* const scene, oct_sceneTreeNodeAtom atom, oct_file oct, bool is_lit)
{
    //Construct the scene node
    ocl_sceneNode node = (ocl_sceneNode){0};

    node.type = OCL_SCENE_NODE_TYPE_MESH;
    //uuid
    node.is_enabled = true;
    node.node_name = _copy_name(oct, atom.node_name);
    glm_mat4_make(atom.local_to_parent_matrix, node.local_to_parent_matrix);

    //Add the scene node to the node table
    scene->node_table[scene->node_count] = node;

    //Link the child
    ocl_link_child(scene, atom.parent_node_refs, scene->node_count);

    //Get the Geometry node object reference
    ocl_sceneNode* parent = scene->node_table[scene->node_count].parent;
    ocl_sceneObject* geom_ref = parent->object_ref;

    //Check if we already loaded the referenced model ID
    ocl_sceneMesh* mesh_ptr = ocl_find_mesh(*scene, geom_ref->mesh_id);

    if(mesh_ptr == NULL)
    {
        //We havent loaded this mesh yet
        ocl_sceneMesh mesh = _load_mesh(geom_ref, is_lit, atom);

        //Add the mesh to the mesh table
        scene->mesh_table[scene->mesh_count] = mesh;

        //Update the empty reference with our newly loaded model
        mesh_ptr = &scene->mesh_table[scene->mesh_count];

        scene->mesh_count++;
    }

    //Update the Geometry nodes mesh reference
    if(geom_ref->mesh_list == NULL)
    {
        //Initialize the mesh list if its empty
        geom_ref->mesh_list = calloc(1, sizeof(ocl_sceneMesh*));
    }
    else
    {
        //Allocate a new spot for the mesh if its already defined
        geom_ref->mesh_list = realloc(geom_ref->mesh_list, (geom_ref->mesh_count+1) * sizeof(ocl_sceneMesh*));
    }

    //Add the submesh to the list
    geom_ref->mesh_list[geom_ref->mesh_count] = mesh_ptr;

    //Increment the counters
    geom_ref->mesh_count++;
    scene->node_count++;
}


ocl_scene ocl_load_oct_scene(oct_sceneDescriptor desc, oct_file oct)
{
    ocl_scene scene = (ocl_scene){0};

    //Preallocate way more than we need
    scene.node_table = calloc(desc.scene_tree_pool_size, sizeof(ocl_sceneNode));
    scene.object_table = calloc(desc.scene_tree_pool_size, sizeof(ocl_sceneObject));
    scene.mesh_table = calloc(desc.scene_tree_pool_size, sizeof(ocl_sceneMesh));

    for(uint32_t i = 0; i < desc.scene_tree_pool_size; i++)
    {
        oct_sceneTreeNodeAtom atom = desc.scene_tree_node_pool[i];

        switch(atom.type)
        {
            case OCT_SCENE_NODE_TYPE_SCENE:
                _load_scene_atom(&scene, atom);
                break;

            case OCT_SCENE_NODE_TYPE_TRANSFORM:
                _load_transform_atom(&scene, atom, oct);
                break;

            case OCT_SCENE_NODE_TYPE_GEOMETRY:
                _load_geometry_atom(&scene, atom, oct);
                break;

            case OCT_SCENE_NODE_TYPE_SUB_GEOMETRY:
                _load_sub_geometry_atom(&scene, atom, oct, false);
                break;

            case OCT_SCENE_NODE_TYPE_SUB_GEOMETRY_LIT:
                _load_sub_geometry_atom(&scene, atom, oct, true);
                break;

            case OCT_SCENE_NODE_TYPE_CAMERA:
            case OCT_SCENE_NODE_TYPE_LIGHT:
                scene.node_count++;
                break;

            default:
                break;
        }

        //printf("Processed node %u\n", i);
    }

    //Shrink the arrays to their actual sizes now
    scene.node_table = realloc(scene.node_table, scene.node_count * sizeof(ocl_sceneNode));
    scene.object_table = realloc(scene.object_table, scene.object_count * sizeof(ocl_sceneObject));
    scene.mesh_table = realloc(scene.mesh_table, scene.mesh_count * sizeof(ocl_sceneMesh));

    printf("Loaded\n|\tNodes: %u\n|\tObjects: %u\n|\tMeshes: %u\n",
           scene.node_count, scene.object_count, scene.mesh_count
    );

    return scene;
}


