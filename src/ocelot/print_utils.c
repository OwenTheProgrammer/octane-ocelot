#include "print_utils.h"
#include "utils.h"
#include <stdint.h>
#include <stdio.h>

static const char* _HEX_TABLE = "0123456789ABCDEF";

static uint16_t _byte_to_hex(uint8_t byte)
{
    uint16_t lo = _HEX_TABLE[byte & 0xF];
    uint16_t hi = _HEX_TABLE[byte >> 4u];
    return lo | (hi << 8);
}

void phex_u8(uint8_t value)
{
    uint16_t data = _byte_to_hex(value);
    printf("0x%c%c", (char)(data>>8u), (char)(data & 0xFF));
}

void phex_u16(uint16_t value)
{
    uint16_t lo = _byte_to_hex(value & 0xFF);
    uint16_t hi = _byte_to_hex(value >> 8u);
    printf("0x%c%c%c%c",
            (char)(hi>>8u), (char)(hi & 0xFF),
            (char)(lo>>8u), (char)(lo & 0xFF)
    );
}


void phex_u8_array(void* const src, uint32_t count)
{
    if(src == NULL)
    {
        printf("(Null)\n");
        return;
    }

    uint8_t* ptr = (uint8_t*)src;

    for(uint32_t i = 0; i < count; i++)
    {
        phex_u8(ptr[i]);
        putchar((i % 8 == 0 && i != 0) ? '\n' : ' ');
    }

    putchar('\n');
}



void oct_print_header(oct_header header)
{
    printf("Oct Header:\n");
    printf("|\tEndian: %s\n", _ENDIAN_PRINT_TABLE[(int)header.endian]);
    printf("|\tString table: %u bytes\n", header.string_table_size);
    printf("|\tData tree: %u bytes\n", header.data_tree_size);
}

void oct_print_atom_header(oct_atomHeader header)
{
    printf("Atom Header:\n");
    printf("|\tType: %s\n", _OCT_ATOM_TYPE_PRINT_TABLE[(int)header.atom_type]);
    printf("|\tHas name: %s\n", header.has_name ? "True" : "False");
    printf("|\tData type: %s\n", _OCT_DATA_TYPE_PRINT_TABLE[(int)header.data_type]);
    printf("|\tLength size: %u B\n", header.length_bytes);
    printf("|\tInt size: %u B\n", header.element_bytes);
    printf("|\tLevel: %u\n", header.tree_level);
}

void oct_print_atom_node(oct_file oct, oct_atomNode node)
{
    printf("Node L%u: \"%s\" %s\n",
           node.header.tree_level,
           oct.string_table[node.st_key].data,
           oct.string_table[node.name_key].data
    );

    printf("|\tType: ");
    switch(node.node_type)
    {
        case OCT_NODE_TYPE_BRANCH:  printf("Branch\n"); break;
        case OCT_NODE_TYPE_STRING:  printf("String\n"); break;
        case OCT_NODE_TYPE_FLOAT:   printf("Float\n"); break;
        case OCT_NODE_TYPE_INT:     printf("Int\n"); break;
        case OCT_NODE_TYPE_UUID:    printf("UUID\n"); break;

        case OCT_NODE_TYPE_STRING_ARRAY:    printf("String[%u]\n", node.elem_count); break;
        case OCT_NODE_TYPE_FLOAT_ARRAY:     printf("Float[%u]\n", node.elem_count); break;
        case OCT_NODE_TYPE_INT_ARRAY:       printf("Int[%u]\n", node.elem_count); break;
        case OCT_NODE_TYPE_BINARY:          printf("Binary[%u]\n", node.elem_count); break;
        default:
            printf("???\n");
            break;
    }

    if(node.elem_count == 1)
    {
        switch(node.node_type)
        {
            case OCT_NODE_TYPE_STRING:
                printf("|\tString[%u]: \"%s\"\n", 
                       *(uint16_t*)node.data, 
                       oct.string_table[*(uint16_t*)node.data].data
                );
                break;

            case OCT_NODE_TYPE_FLOAT:
                printf("|\tFloat: %f\n", *(float*)node.data);
                break;

            case OCT_NODE_TYPE_INT:
                printf("|\tInt: %i\n", *(int32_t*)node.data);
                break;

            case OCT_NODE_TYPE_BINARY:
                printf("|\tBinary[%u]:\n", node.elem_count);
                phex_u8_array(node.data, node.elem_count);
                printf("-----------\n");
                break;

            case OCT_NODE_TYPE_UUID:
                printf("|\tUUID: ");
                phex_u8_array(node.data, node.elem_count);
                break;

            default:
                break;
        }
    }
    else if(node.elem_count > 1)
    {
        for(uint32_t i = 0; i < node.elem_count; i++)
        {
            switch(node.node_type)
            {
                case OCT_NODE_TYPE_STRING_ARRAY:
                    printf("|\t%u: String[%u]: \"%s\"\n",
                           i,
                           ((uint16_t*)node.data)[i],
                           oct.string_table[((uint16_t*)node.data)[i]].data
                    );
                    break;

                case OCT_NODE_TYPE_FLOAT_ARRAY:
                    printf("| %u: %f\n", i, ((float*)node.data)[i]);
                    break;

                case OCT_NODE_TYPE_INT_ARRAY:
                    printf("| %u: %i\n", i, ((int32_t*)node.data)[i]);
                    break;

                default:
                    break;
            }
        }
    }

    printf("--------------\n");
}

void oct_print_atom_index_stream(oct_file oct, oct_indexStreamAtom atom)
{
    printf("IndexStream %s\n", oct.string_table[atom.atom_id].data);
    printf("|\tLength: %u\n", atom.length);
    printf("|\tBufferReference: %u\n", atom.buffer_reference);
    printf("|\tBufferOffset: %u\n", atom.buffer_offset);
    printf("|\tBufferPrimitive: Triangles\n");
}

void oct_print_atom_vertex_stream(oct_file oct, oct_vertexStreamAtom atom)
{
    printf("VertexStream %s\n", oct.string_table[atom.atom_id].data);
    printf("|\tLength: %u\n", atom.length);
    printf("|\tWidth: %u\n", atom.width);
    printf("|\tVertexBufferReference: %u\n", atom.buffer_reference);
    printf("|\tVertexBufferOffset: %u\n", atom.buffer_offset);
    printf("|\tElements[%u]:\n", atom.element_count);

    for(uint32_t i = 0; i < atom.element_count; i++)
    {
        oct_print_atom_vertex_element(oct, atom.elements[i]);
    }
}

void oct_print_atom_vertex_element(oct_file oct, oct_vstreamElementAtom atom)
{
    printf("Element %s\n", oct.string_table[atom.atom_id].data);
    printf("|\tType: %u\n", atom.type);
    printf("|\tName: %s\n", oct.string_table[atom.name].data);
    printf("|\tOffset: %u\n", atom.offset);
}


void oct_print_atom_scene_geometry(oct_file oct, oct_sceneGeometryNode atom)
{
    printf("Geometry:\n");
    printf("|\tVisible: %s\n", atom.visible ? "True" : "False");
    printf("|\tMeshName: %s\n", oct.string_table[atom.mesh_name].data);
}

void oct_print_atom_scene_sub_geometry(oct_file oct, oct_sceneSubGeometryNode atom)
{
    printf("SubGeometry:\n");
    printf("|\tMaterialReference: %u\n", atom.material_reference);
    printf("|\tVertexStreamReferences[%u]:\n", atom.vstream_count);
    for(uint32_t i = 0; i < atom.vstream_count; i++)
    {
        printf("|\t - %u\n", atom.vstream_refs[i]);
    }
    printf("|\tIndexStreamReference: %u\n", atom.istream_ref);
}

void oct_print_atom_scene_sub_geometry_lit(oct_file oct, oct_sceneSubGeometryNode atom)
{
    printf("SubGeometryLit\n");
    printf("|\tMaterialReference: %u\n", atom.material_reference);
    printf("|\tVertexStreamReferences[%u]:\n", atom.vstream_count);
    for(uint32_t i = 0; i < atom.vstream_count; i++)
    {
        printf("|\t - %u\n", atom.vstream_refs[i]);
    }
    printf("|\tIndexStreamReference: %u\n", atom.istream_ref);

}

void oct_print_atom_scene_camera(oct_file oct, oct_sceneCameraNode atom)
{
    printf("Camera\n");
    printf("|\tIsOrthographic: %s\n", atom.is_orthographic ? "True" : "False");
    printf("|\tFocalLength: %f\n", atom.focal_length);
    printf("|\tCameraScale: %f\n", atom.scale);
    printf("|\tNearClip: %f\n", atom.near_clip);
    printf("|\tFarClip: %f\n", atom.far_clip);
    printf("|\tHorizontalFilmAperture: %f\n", atom.horizontal_film_aperture);
    printf("|\tVerticalFilmAperture: %f\n", atom.vertical_film_aperture);
    printf("|\tLensSqueezeRatio: %f\n", atom.lens_squeeze_ratio);
}

void oct_print_atom_scene_light(oct_file oct, oct_sceneLightNode atom)
{
    printf("Light\n");
    printf("|\tType: %u\n", (uint32_t)atom.type);
    printf("|\tLightColor RGB: %u, %u, %u\n", 
           (uint32_t)(atom.light_color.x * 255),
           (uint32_t)(atom.light_color.y * 255),
           (uint32_t)(atom.light_color.z * 255)
    );
    printf("|\tLightIntensity: %f\n", atom.light_intensity);
    printf("|\tAmbientShade: %f\n", atom.ambient_shade);
    printf("|\tShadowColor RGB: %u, %u, %u\n",
           (uint32_t)(atom.shadow_color.x * 255),
           (uint32_t)(atom.shadow_color.y * 255),
           (uint32_t)(atom.shadow_color.z * 255)
    );
}

void oct_print_scene_tree_node(oct_file oct, oct_sceneTreeNodeAtom atom)
{
    printf("Node %s\n", oct.string_table[atom.atom_id].data);
    printf("|\tNodeName: %s\n", oct.string_table[atom.node_name].data);
    printf("|\tType: %u\n", (uint32_t)atom.type);
    printf("|\tParentNodeRef: %u\n", atom.parent_node_refs);
    //printf("|\tLocalToParentMatrix\n");
    switch(atom.type)
    {
        case OCT_SCENE_NODE_TYPE_GEOMETRY:
            oct_print_atom_scene_geometry(oct, atom.geometry);
            break;

        case OCT_SCENE_NODE_TYPE_SUB_GEOMETRY:
            oct_print_atom_scene_sub_geometry(oct, atom.sub_geometry);
            break;

        case OCT_SCENE_NODE_TYPE_SUB_GEOMETRY_LIT:
            oct_print_atom_scene_sub_geometry(oct, atom.sub_geometry);
            break;

        case OCT_SCENE_NODE_TYPE_CAMERA:
            oct_print_atom_scene_camera(oct, atom.camera);
            break;

        case OCT_SCENE_NODE_TYPE_LIGHT:
            oct_print_atom_scene_light(oct, atom.light);
            break;

        default:
            break;
    }
}

void oct_print_scene_descriptor(oct_file oct, oct_sceneDescriptor scene)
{
    for(uint32_t i = 0; i < scene.istream_pool_size; i++)
    {
        oct_print_atom_index_stream(oct, scene.istream_pool[i]);
    }
    for(uint32_t i = 0; i < scene.vstream_pool_size; i++)
    {
        oct_print_atom_vertex_stream(oct, scene.vstream_pool[i]);
    }
    for(uint32_t i = 0; i < scene.scene_tree_pool_size; i++)
    {
        oct_print_scene_tree_node(oct, scene.scene_tree_node_pool[i]);
    }
}


#define PRINT_ST(x) printf("|\t\"%s\": StringTable[%u]\n", oct.string_table[x].data, x);

void oct_print_atom_name_table(oct_file oct, oct_atomNameTable ant)
{
    printf("AtomNameTable:\n");
    PRINT_ST(ant.length)
    PRINT_ST(ant.width)
    PRINT_ST(ant.type)
    PRINT_ST(ant.offset)
    PRINT_ST(ant.name)

    PRINT_ST(ant.index_stream_pool)
    PRINT_ST(ant.index_stream)
    PRINT_ST(ant.index_buffer_reference)
    PRINT_ST(ant.index_buffer_offset)
    PRINT_ST(ant.index_stream_primitive)

    PRINT_ST(ant.vertex_stream_pool)
    PRINT_ST(ant.vertex_stream)
    PRINT_ST(ant.vstream_buffer_reference)
    PRINT_ST(ant.vstream_buffer_offset)
    PRINT_ST(ant.vstream_elements)
    PRINT_ST(ant.vstream_element)

    PRINT_ST(ant.vstream_element_name_position)
    PRINT_ST(ant.vstream_element_name_uv1)
    PRINT_ST(ant.vstream_element_name_tangent)
    PRINT_ST(ant.vstream_element_name_normal)
    PRINT_ST(ant.vstream_element_name_binormal)
    PRINT_ST(ant.vstream_element_name_lightmapUV)
    PRINT_ST(ant.vstream_element_name_color_add)
    PRINT_ST(ant.vstream_element_name_color_occl)
    PRINT_ST(ant.vstream_element_name_vertex_baked)

    PRINT_ST(ant.scene_tree_node_pool)
    PRINT_ST(ant.scene_node)
    PRINT_ST(ant.scene_node_type_scene)
    PRINT_ST(ant.scene_node_type_geometry)
    PRINT_ST(ant.scene_node_type_sub_geometry)
    PRINT_ST(ant.scene_node_type_sub_geometry_lit)
    PRINT_ST(ant.scene_node_type_camera)
    PRINT_ST(ant.scene_node_type_light)
    PRINT_ST(ant.scene_node_name)
    PRINT_ST(ant.scene_node_parent_node_refs)
    PRINT_ST(ant.scene_node_local_to_parent_matrix)
    PRINT_ST(ant.scene_node_visible)
    PRINT_ST(ant.scene_node_mesh_name)
    PRINT_ST(ant.scene_node_material_ref)
    PRINT_ST(ant.scene_node_vstream_refs)
    PRINT_ST(ant.scene_node_istream_ref)
    PRINT_ST(ant.scene_node_is_orthographic)
    PRINT_ST(ant.scene_node_focal_length)
    PRINT_ST(ant.scene_node_camera_scale)
    PRINT_ST(ant.scene_node_near_clip)
    PRINT_ST(ant.scene_node_far_clip)
    PRINT_ST(ant.scene_node_horizontal_film_aperture)
    PRINT_ST(ant.scene_node_vertical_film_aperture)
    PRINT_ST(ant.scene_node_lens_squeeze_ratio)
    PRINT_ST(ant.scene_node_light_type)
    PRINT_ST(ant.scene_node_light_color)
    PRINT_ST(ant.scene_node_light_intensity)
    PRINT_ST(ant.scene_node_shadow_color)
    PRINT_ST(ant.scene_node_ambient_shade)

    PRINT_ST(ant.scene_node_light_type_directional)
    PRINT_ST(ant.scene_node_light_type_point)
    PRINT_ST(ant.scene_node_light_type_ambient)
}

#undef PRINT_ST


const char* _OCT_ATOM_TYPE_PRINT_TABLE[OCT_ATOM_TYPE_MAX] =
{
    "Unknown",
    "Container",
    "List",
    "Element"
};

const char* _OCT_DATA_TYPE_PRINT_TABLE[OCT_DATA_TYPE_MAX] =
{
    "None",
    "String",
    "Float",
    "Int",
    "Binary"
};
