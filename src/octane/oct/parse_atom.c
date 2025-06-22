#include "internal.h"
#include "octane/oct/oct.h"
#include "octane/oct/atoms.h"
#include "octane/oct/name_bindings.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


oct_vertexBufferAtom _oct_atom_read_vertex_buffer(oct_file oct, uint32_t start_idx, uint32_t end_idx)
{
    oct_vertexBufferAtom atom = (oct_vertexBufferAtom){0};

    atom.atom_id = oct.data_tree[start_idx].name_key;

    for(uint32_t i = start_idx+1; i < end_idx; i++)
    {
        oct_atomNode node = oct.data_tree[i];

        if(node.st_key == _oct_ant.VertexBufferPool.VertexBuffer.Name)
        {
            atom.name = *(uint16_t*)node.data;
        }
        else if(node.st_key == _oct_ant.VertexBufferPool.VertexBuffer.Flags)
        {
            atom.flags = *(uint32_t*)node.data;
        }
        else if(node.st_key == _oct_ant.VertexBufferPool.VertexBuffer.Size)
        {
            atom.size = *(uint32_t*)node.data;
        }
        else if(node.st_key == _oct_ant.VertexBufferPool.VertexBuffer.HeapLoc)
        {
            atom.heap_loc = *(uint32_t*)node.data;
        }
        else if(node.st_key == _oct_ant.VertexBufferPool.VertexBuffer.FileName)
        {
            atom.file_name = *(uint16_t*)node.data;
        }
    }

    return atom;
}

oct_indexBufferAtom _oct_atom_read_index_buffer(oct_file oct, uint32_t start_idx, uint32_t end_idx)
{
    oct_indexBufferAtom atom = (oct_indexBufferAtom){0};

    atom.atom_id = oct.data_tree[start_idx].name_key;

    for(uint32_t i = start_idx+1; i < end_idx; i++)
    {
        oct_atomNode node = oct.data_tree[i];

        if(node.st_key == _oct_ant.IndexBufferPool.IndexBuffer.Width)
        {
            atom.width = *(uint32_t*)node.data;
        }
        else if(node.st_key == _oct_ant.IndexBufferPool.IndexBuffer.Name)
        {
            atom.name = *(uint16_t*)node.data;
        }
        else if(node.st_key == _oct_ant.IndexBufferPool.IndexBuffer.Flags)
        {
            atom.flags = *(uint16_t*)node.data;
        }
        else if(node.st_key == _oct_ant.IndexBufferPool.IndexBuffer.Size)
        {
            atom.size = *(uint32_t*)node.data;
        }
        else if(node.st_key == _oct_ant.IndexBufferPool.IndexBuffer.FileName)
        {
            atom.file_name = *(uint16_t*)node.data;
        }
    }

    return atom;
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
