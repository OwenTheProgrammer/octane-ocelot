#pragma once
#include "octane/oct.h"
#include <stdbool.h>
#include <stdint.h>
#include <cglm/vec3.h>
#include <cglm/mat4.h>

typedef enum
{
    OCT_SCENE_NODE_TYPE_NONE,
    OCT_SCENE_NODE_TYPE_SCENE,
    OCT_SCENE_NODE_TYPE_TRANSFORM,
    OCT_SCENE_NODE_TYPE_GEOMETRY,
    OCT_SCENE_NODE_TYPE_SUB_GEOMETRY,
    OCT_SCENE_NODE_TYPE_SUB_GEOMETRY_LIT,
    OCT_SCENE_NODE_TYPE_CAMERA,
    OCT_SCENE_NODE_TYPE_LIGHT,
    OCT_SCENE_NODE_TYPE_MAX
} oct_sceneNodeType;

typedef enum
{
    OCT_SCENE_LIGHT_TYPE_NONE,
    OCT_SCENE_LIGHT_TYPE_DIRECTIONAL,
    OCT_SCENE_LIGHT_TYPE_POINT,
    OCT_SCENE_LIGHT_TYPE_AMBIENT,
    OCT_SCENE_LIGHT_TYPE_MAX
} oct_sceneLightType;

typedef enum
{
    OCT_ELEMENT_TYPE_FLOAT_ONE      = 0,
    OCT_ELEMENT_TYPE_FLOAT_TWO      = 1, //educated guess
    OCT_ELEMENT_TYPE_FLOAT_THREE    = 2,
    OCT_ELEMENT_TYPE_FLOAT_FOUR     = 3,
    OCT_ELEMENT_TYPE_INT_ONE        = 10, //assuming this is int
    OCT_ELEMENT_TYPE_RGBA_U8        = 25,
    OCT_ELEMENT_TYPE_PACKED_VECTOR  = 39, //S10_S11_S11?
    OCT_ELEMENT_TYPE_TEXCOORD       = 44, //R16_G16_UNORM?
} oct_elementType;

typedef enum
{
    OCT_VSTREAM_ELEMENT_NAME_NONE           = 0,
    OCT_VSTREAM_ELEMENT_NAME_POSITION       = 1<<0,
    OCT_VSTREAM_ELEMENT_NAME_UV1            = 1<<1,
    OCT_VSTREAM_ELEMENT_NAME_TANGENT        = 1<<2,
    OCT_VSTREAM_ELEMENT_NAME_NORMAL         = 1<<3,
    OCT_VSTREAM_ELEMENT_NAME_BINORMAL       = 1<<4,
    OCT_VSTREAM_ELEMENT_NAME_LIGHTMAP_UV    = 1<<5,
    OCT_VSTREAM_ELEMENT_NAME_COLOR_ADD      = 1<<6,
    OCT_VSTREAM_ELEMENT_NAME_COLOR_OCC      = 1<<7,
    OCT_VSTREAM_ELEMENT_NAME_VERTEX_BAKED   = 1<<8,
    OCT_VSTREAM_ELEMENT_NAME_MAX            = 10
} oct_vstreamElementName;

typedef enum
{
    OCT_PRIMITIVE_TYPE_NONE,
    OCT_PRIMITIVE_TYPE_TRIANGLES,
    OCT_PRIMITIVE_TYPE_MAX
} oct_primitiveType;

typedef struct oct_atomNameTable    oct_atomNameTable;

typedef struct oct_indexStreamAtom      oct_indexStreamAtom;
typedef struct oct_vertexStreamAtom     oct_vertexStreamAtom;
typedef struct oct_vstreamElementAtom   oct_vstreamElementAtom;

typedef struct oct_sceneTreeNodeAtom    oct_sceneTreeNodeAtom;

typedef struct oct_sceneCameraNode      oct_sceneCameraNode;
typedef struct oct_sceneGeometryNode    oct_sceneGeometryNode;
typedef struct oct_sceneSubGeometryNode oct_sceneSubGeometryNode;
typedef struct oct_sceneLightNode       oct_sceneLightNode;

struct oct_atomNameTable
{
    /* Length */
    uint16_t length;

    /* Width */
    uint16_t width;

    /* Type */
    uint16_t type;

    /* Offset */
    uint16_t offset;

    /* Name */
    uint16_t name;


    /* IndexStreamPool */
    uint16_t index_stream_pool;

    /* IndexStreamPool/IndexStream */
    uint16_t index_stream;

    /* IndexStreamPool/IndexStream/IndexBufferReference */
    uint16_t index_buffer_reference;

    /* IndexStreamPool/IndexStream/IndexBufferOffset */
    uint16_t index_buffer_offset;

    /* IndexStreamPool/IndexStream/IndexStreamPrimitive */
    uint16_t index_stream_primitive;


    /* VertexStreamPool */
    uint16_t vertex_stream_pool;

    /* VertexStreamPool/VertexStream */
    uint16_t vertex_stream;

    /* VertexStreamPool/VertexStream/VertexBufferReference */
    uint16_t vstream_buffer_reference;

    /* VertexStreamPool/VertexStream/VertexBufferOffset */
    uint16_t vstream_buffer_offset;

    /* VertexStreamPool/VertexStream/Elements */
    uint16_t vstream_elements;

    /* VertexStreamPool/VertexStream/Elements/Element */
    uint16_t vstream_element;

    /* VertexStreamPool/VertexStream/Elements/Element/Name:Position */
    uint16_t vstream_element_name_position;

    /* VertexStreamPool/VertexStream/Elements/Element/Name:Uv1 */
    uint16_t vstream_element_name_uv1;

    /* VertexStreamPool/VertexStream/Elements/Element/Name:Tangent */
    uint16_t vstream_element_name_tangent;

    /* VertexStreamPool/VertexStream/Elements/Element/Name:Normal */
    uint16_t vstream_element_name_normal;

    /* VertexStreamPool/VertexStream/Elements/Element/Name:Binormal */
    uint16_t vstream_element_name_binormal;

    /* VertexStreamPool/VertexStream/Elements/Element/Name:lightmapUV */
    uint16_t vstream_element_name_lightmapUV;

    /* VertexStreamPool/VertexStream/Elements/Element/Name:ColorAdditive */
    uint16_t vstream_element_name_color_add;

    /* VertexStreamPool/VertexStream/Elements/Element/Name:ColorOcclusion */
    uint16_t vstream_element_name_color_occl;

    /* VertexStreamPool/VertexStream/Elements/Element/Name:VertexBaked */
    uint16_t vstream_element_name_vertex_baked;


    /* SceneTreeNodePool */
    uint16_t scene_tree_node_pool;

    /* SceneTreeNodePool/Node */
    uint16_t scene_node;

    /* SceneTreeNodePool/Node/Type:Scene */
    uint16_t scene_node_type_scene;

    /* SceneTreeNodePool/Node/Type:Transform */
    uint16_t scene_node_type_transform;

    /* SceneTreeNodePool/Node/Type:Geometry */
    uint16_t scene_node_type_geometry;

    /* SceenTreeNodePool/Node/Type:SubGeometry */
    uint16_t scene_node_type_sub_geometry;

    /* SceneTreeNodePool/Node/Type:SubGeometryLit */
    uint16_t scene_node_type_sub_geometry_lit;

    /* SceneTreeNodePool/Node/Type:Camera */
    uint16_t scene_node_type_camera;

    /* SceneTreeNodePool/Node/Type:Light */
    uint16_t scene_node_type_light;

    /* SceenTreeNodePool/Node/NodeName */
    uint16_t scene_node_name;

    /* SceneTreeNodePool/Node/ParentNodeReferences */
    uint16_t scene_node_parent_node_refs;

    /* SceneTreeNodePool/Node/LocalToParentMatrix */
    uint16_t scene_node_local_to_parent_matrix;

    /* SceneTreeNodePool/Node/Visible */
    uint16_t scene_node_visible;

    /* SceneTreeNodePool/Node/MeshName */
    uint16_t scene_node_mesh_name;

    /* SceneTreeNodePool/Node/MaterialReference */
    uint16_t scene_node_material_ref;

    /* SceneTreeNodePool/Node/VertexStreamReferences */
    uint16_t scene_node_vstream_refs;

    /* SceneTreeNodePool/Node/IndexStreamReference */
    uint16_t scene_node_istream_ref;

    /* SceneTreeNodePool/Node/IsOrthographic */
    uint16_t scene_node_is_orthographic;

    /* SceneTreeNodePool/Node/FocalLength */
    uint16_t scene_node_focal_length;

    /* SceneTreeNodePool/Node/CameraScale */
    uint16_t scene_node_camera_scale;

    /* SceneTreeNodePool/Node/NearClipPlane */
    uint16_t scene_node_near_clip;

    /* SceneTreeNodePool/Node/FarClipPlane */
    uint16_t scene_node_far_clip;

    // NOTE: The octane format has a spelling mistake for aperture

    /* SceneTreeNodePool/Node/HorizontalFilmAmperture */
    uint16_t scene_node_horizontal_film_aperture;

    /* SceneTreeNodePool/Node/VerticalFilmAperture */
    uint16_t scene_node_vertical_film_aperture;

    /* SceneTreeNodePool/Node/LensSqueezeRatio */
    uint16_t scene_node_lens_squeeze_ratio;

    /* SceneTreeNodePool/Node/LightType */
    uint16_t scene_node_light_type;

    /* SceneTreeNodePool/Node/LightColor */
    uint16_t scene_node_light_color;

    /* SceneTreeNodePool/Node/LightIntensity */
    uint16_t scene_node_light_intensity;

    /* SceneTreeNodePool/Node/ShadowColor */
    uint16_t scene_node_shadow_color;

    /* SceneTreeNodePool/Node/AmbientShade */
    uint16_t scene_node_ambient_shade;

    /* SceneTreeNodePool/Node/Light Type:Directional */
    uint16_t scene_node_light_type_directional;

    /* SceneTreeNodePool/Node/Light Type:Point */
    uint16_t scene_node_light_type_point;

    /* SceneTreeNodePool/Node/Light Type:Ambient */
    uint16_t scene_node_light_type_ambient;
};

extern oct_atomNameTable _oct_ant;

/* Serialization of IndexStreamPool/IndexStream */
struct oct_indexStreamAtom
{
    uint16_t atom_id;

    uint32_t length;

    uint32_t buffer_reference;

    uint32_t buffer_offset;

    oct_primitiveType stream_primitive;
};

/* Serialization of VertexStreamPool/VertexStream */
struct oct_vertexStreamAtom
{
    uint16_t atom_id;

    /* The amount of elements in the vertex stream */
    uint32_t length;

    /* The stride of a vertex in bytes */
    uint32_t width;

    /* The VertexBuffers index from the VertexBufferPool */
    uint32_t buffer_reference;

    /* Starting offset into the vbuf file in bytes */
    uint32_t buffer_offset;

    /* Array size of elements array */
    uint32_t element_count;

    /* Bitflags for each element this vertex stream contains */
    oct_vstreamElementName element_flags;

    /* Array of elements for this vertex stream */
    oct_vstreamElementAtom* elements;
};

/* Serialization of VertexStreamPool/VertexStream/Elements/Element */
struct oct_vstreamElementAtom
{
    uint16_t atom_id;

    oct_elementType type;

    uint16_t name;

    oct_vstreamElementName attribute_type;

    uint32_t offset;
};


/* Serialization of SceneTreeNodePool/Node/Type:Geometry */
struct oct_sceneGeometryNode
{
    bool visible;

    uint16_t mesh_name;
};

/* Serialization of SceneTreeNodePool/Node/Type:{SubGeometry, SubGeometryLit} */
struct oct_sceneSubGeometryNode
{
    uint32_t material_reference;

    uint32_t vstream_count;

    uint32_t* vstream_refs;

    uint32_t istream_ref;
};

/* Serialization of SceneTreeNodePool/Node/Type:Camera */
struct oct_sceneCameraNode
{
    bool is_orthographic;

    float focal_length;

    float scale;

    float near_clip;

    float far_clip;

    float horizontal_film_aperture;

    float vertical_film_aperture;

    float lens_squeeze_ratio;
};

/* Serialization of SceneTreeNodePool/Node/Type:Light */
struct oct_sceneLightNode
{
    oct_sceneLightType type;

    vec3 light_color;

    float light_intensity;

    float ambient_shade;

    vec3 shadow_color;
};


/* Serialization of SceneTreeNodePool/Node */
struct oct_sceneTreeNodeAtom
{
    uint16_t atom_id;

    uint16_t node_name;

    oct_sceneNodeType type;

    // TODO: This is prone to failure if theres more than one parent ref.
    // I dont know when that would happen though
    uint32_t parent_node_refs;

    mat4 local_to_parent_matrix;

    /* == Node Types == */

    oct_sceneGeometryNode geometry;

    oct_sceneSubGeometryNode sub_geometry;

    oct_sceneCameraNode camera;

    oct_sceneLightNode light;
};


/* Returns the string table index of a string, 0 if not found */
uint16_t oct_get_st_index_of(oct_file oct, const char* str);

uint32_t oct_get_atom_node_index(oct_file oct, uint16_t key);


oct_atomNameTable oct_create_atom_name_table(oct_file oct);

void oct_init_atom_name_table(oct_file oct);

