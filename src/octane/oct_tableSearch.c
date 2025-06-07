#include "octane/oct.h"
#include "octane/oct_atoms.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

oct_atomNameTable _oct_ant = (oct_atomNameTable){0};

uint16_t oct_get_st_index_of(oct_file oct, const char* str)
{
    if(str == NULL)
        return 0;

    uint32_t in_len = strlen(str);

    if(in_len == 0)
        return 0;

    for(uint32_t i = 0; i < oct.string_table_length; i++)
    {
        oct_string element = oct.string_table[i];

        if(element.len == in_len && strncmp(str, element.data, element.len) == 0)
            return (uint16_t)i;
    }

    return 0;
}


uint32_t oct_get_atom_node_index(oct_file oct, uint16_t key)
{
    if(key == 0)
        return 0;

    for(uint32_t i = 0; i < oct.data_tree_length; i++)
    {
        oct_atomNode node = oct.data_tree[i];

        if(node.st_key == key)
            return i;
    }

    return 0;
}


oct_atomNameTable oct_create_atom_name_table(oct_file oct)
{
    oct_atomNameTable ant = (oct_atomNameTable){0};

    ant.length = oct_get_st_index_of(oct, "Length");

    ant.width = oct_get_st_index_of(oct, "Width");

    ant.type = oct_get_st_index_of(oct, "Type");

    ant.offset = oct_get_st_index_of(oct, "Offset");

    ant.name = oct_get_st_index_of(oct, "Name");


    ant.index_stream_pool = oct_get_st_index_of(oct, "IndexStreamPool");

    ant.index_stream = oct_get_st_index_of(oct, "IndexStream");

    ant.index_buffer_reference = oct_get_st_index_of(oct, "IndexBufferReference");

    ant.index_buffer_offset = oct_get_st_index_of(oct, "IndexBufferOffset");

    ant.index_stream_primitive = oct_get_st_index_of(oct, "IndexStreamPrimitive");


    ant.vertex_stream_pool = oct_get_st_index_of(oct, "VertexStreamPool");

    ant.vertex_stream = oct_get_st_index_of(oct, "VertexStream");

    ant.vstream_buffer_reference = oct_get_st_index_of(oct, "VertexBufferReference");

    ant.vstream_buffer_offset = oct_get_st_index_of(oct, "VertexBufferOffset");

    ant.vstream_elements = oct_get_st_index_of(oct, "Elements");

    ant.vstream_element = oct_get_st_index_of(oct, "Element");


    ant.vstream_element_name_position = oct_get_st_index_of(oct, "Position");

    ant.vstream_element_name_uv1 = oct_get_st_index_of(oct, "Uv1");

    ant.vstream_element_name_tangent = oct_get_st_index_of(oct, "Tangent");

    ant.vstream_element_name_normal = oct_get_st_index_of(oct, "Normal");

    ant.vstream_element_name_binormal = oct_get_st_index_of(oct, "Binormal");

    ant.vstream_element_name_lightmapUV = oct_get_st_index_of(oct, "lightmapUV");

    ant.vstream_element_name_color_add = oct_get_st_index_of(oct, "ColorAdditive");

    ant.vstream_element_name_color_occl = oct_get_st_index_of(oct, "ColorOcclusion");

    ant.vstream_element_name_vertex_baked = oct_get_st_index_of(oct, "VertexBaked");


    ant.scene_tree_node_pool = oct_get_st_index_of(oct, "SceneTreeNodePool");

    ant.scene_node = oct_get_st_index_of(oct, "Node");

    ant.scene_node_type_scene = oct_get_st_index_of(oct, "Scene");

    ant.scene_node_type_transform = oct_get_st_index_of(oct, "Transform");

    ant.scene_node_type_geometry = oct_get_st_index_of(oct, "Geometry");

    ant.scene_node_type_sub_geometry = oct_get_st_index_of(oct, "SubGeometry");

    ant.scene_node_type_sub_geometry_lit = oct_get_st_index_of(oct, "SubGeometryLit");

    ant.scene_node_type_camera = oct_get_st_index_of(oct, "Camera");

    ant.scene_node_type_light = oct_get_st_index_of(oct, "Light");

    ant.scene_node_name = oct_get_st_index_of(oct, "NodeName");

    ant.scene_node_parent_node_refs = oct_get_st_index_of(oct, "ParentNodeReferences");

    ant.scene_node_local_to_parent_matrix = oct_get_st_index_of(oct, "LocalToParentMatrix");

    ant.scene_node_visible = oct_get_st_index_of(oct, "Visible");

    ant.scene_node_mesh_name = oct_get_st_index_of(oct, "MeshName");

    ant.scene_node_material_ref = oct_get_st_index_of(oct, "MaterialReference");

    ant.scene_node_vstream_refs = oct_get_st_index_of(oct, "VertexStreamReferences");

    ant.scene_node_istream_ref = oct_get_st_index_of(oct, "IndexStreamReference");

    ant.scene_node_is_orthographic = oct_get_st_index_of(oct, "IsOrthographic");

    ant.scene_node_focal_length = oct_get_st_index_of(oct, "FocalLength");

    ant.scene_node_camera_scale = oct_get_st_index_of(oct, "CameraScale");

    ant.scene_node_near_clip = oct_get_st_index_of(oct, "NearClipPlane");

    ant.scene_node_far_clip = oct_get_st_index_of(oct, "FarClipPlane");

    ant.scene_node_horizontal_film_aperture = oct_get_st_index_of(oct, "HorizontalFilmAmperture");

    ant.scene_node_vertical_film_aperture = oct_get_st_index_of(oct, "VerticalFilmAperture");

    ant.scene_node_lens_squeeze_ratio = oct_get_st_index_of(oct, "LensSqueezeRatio");

    ant.scene_node_light_type = oct_get_st_index_of(oct, "LightType");

    ant.scene_node_light_color = oct_get_st_index_of(oct, "LightColor");

    ant.scene_node_light_intensity = oct_get_st_index_of(oct, "LightIntensity");

    ant.scene_node_shadow_color = oct_get_st_index_of(oct, "ShadowColor");

    ant.scene_node_ambient_shade = oct_get_st_index_of(oct, "AmbientShade");

    ant.scene_node_light_type_directional = oct_get_st_index_of(oct, "Directional");

    ant.scene_node_light_type_point = oct_get_st_index_of(oct, "Point");

    ant.scene_node_light_type_ambient = oct_get_st_index_of(oct, "Ambient");

    return ant;
}

void oct_init_atom_name_table(oct_file oct)
{
    _oct_ant = oct_create_atom_name_table(oct);
}

