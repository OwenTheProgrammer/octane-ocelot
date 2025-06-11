#pragma once
#include <stdbool.h>
#include <stdint.h>

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


typedef struct oct_indexStreamAtom      oct_indexStreamAtom;
typedef struct oct_vertexStreamAtom     oct_vertexStreamAtom;
typedef struct oct_vstreamElementAtom   oct_vstreamElementAtom;

typedef struct oct_sceneTreeNodeAtom    oct_sceneTreeNodeAtom;

typedef struct oct_sceneCameraNode      oct_sceneCameraNode;
typedef struct oct_sceneGeometryNode    oct_sceneGeometryNode;
typedef struct oct_sceneSubGeometryNode oct_sceneSubGeometryNode;
typedef struct oct_sceneLightNode       oct_sceneLightNode;


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

    float light_color[3];

    float light_intensity;

    float ambient_shade;

    float shadow_color[3];
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

    float local_to_parent_matrix[16];

    /* == Node Types == */

    oct_sceneGeometryNode geometry;

    oct_sceneSubGeometryNode sub_geometry;

    oct_sceneCameraNode camera;

    oct_sceneLightNode light;
};

