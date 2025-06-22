#pragma once
#include "octane/oct/enums.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct oct_vertexBufferAtom     oct_vertexBufferAtom;
typedef struct oct_indexBufferAtom      oct_indexBufferAtom;

typedef struct oct_indexStreamAtom      oct_indexStreamAtom;
typedef struct oct_vertexStreamAtom     oct_vertexStreamAtom;
typedef struct oct_vstreamElementAtom   oct_vstreamElementAtom;

typedef struct oct_sceneTreeNodeAtom    oct_sceneTreeNodeAtom;

typedef struct oct_sceneCameraNode      oct_sceneCameraNode;
typedef struct oct_sceneGeometryNode    oct_sceneGeometryNode;
typedef struct oct_sceneSubGeometryNode oct_sceneSubGeometryNode;
typedef struct oct_sceneLightNode       oct_sceneLightNode;

/* Serialization of VertexBufferPool/VertexBuffer */
struct oct_vertexBufferAtom
{
    uint16_t atom_id;

    uint16_t name;

    uint32_t flags;

    uint32_t size;

    uint32_t heap_loc;

    uint16_t file_name;
};

/* Serialization of IndexBufferPool/IndexBuffer */
struct oct_indexBufferAtom
{
    uint16_t atom_id;

    uint32_t width;

    uint16_t name;

    uint32_t flags;

    uint32_t size;

    uint16_t file_name;
};

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
    ocl_vertexElementType element_flags;

    /* Array of elements for this vertex stream */
    oct_vstreamElementAtom* elements;
};

/* Serialization of VertexStreamPool/VertexStream/Elements/Element */
struct oct_vstreamElementAtom
{
    uint16_t atom_id;

    oct_elementType type;

    uint16_t name;

    ocl_vertexElementType attribute_type;

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

