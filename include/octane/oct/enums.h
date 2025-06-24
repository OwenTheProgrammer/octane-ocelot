/* All the enum types derived from the .oct file format */
#pragma once

#ifndef __cplusplus

/* == OCTANE ATOM ENUMS == */

typedef enum oct_atomType   oct_atomType;
typedef enum oct_dataType   oct_dataType;

typedef enum oct_sceneNodeType  oct_sceneNodeType;
typedef enum oct_sceneLightType oct_sceneLightType;

typedef enum oct_elementType    oct_elementType;
typedef enum oct_primitiveType  oct_primitiveType;

/* == OCELOT ENUMS == */

typedef enum ocl_nodeType           ocl_nodeType;
typedef enum ocl_vertexElementType  ocl_vertexElementType;

#endif


/* Implements the type of an oct atom */
enum oct_atomType
{
    OCT_ATOM_TYPE_UNKNOWN   = 0b00, /* Unknown or invalid atom */
    OCT_ATOM_TYPE_CONTAINER = 0b01, /* Atom containing a list of child atoms */
    OCT_ATOM_TYPE_LIST      = 0b10, /* Atom containing a list of elements or values */
    OCT_ATOM_TYPE_ELEMENT   = 0b11, /* Atom containing a single element or value */
    OCT_ATOM_TYPE_MAX
};

/* Implements the type of an oct atoms element */
enum oct_dataType
{
    OCT_DATA_TYPE_NONE      = 0b000,    /* No element type associated */
    OCT_DATA_TYPE_STRING    = 0b001,    /* Value representing a string */
    OCT_DATA_TYPE_FLOAT     = 0b010,    /* Value representing a floating point number */
    OCT_DATA_TYPE_INT       = 0b011,    /* Value representing a signed integer number */
    OCT_DATA_TYPE_BINARY    = 0b100,    /* Raw stream of binary */
    OCT_DATA_TYPE_MAX
};

#define NODE_TYPE(AT, DT) ( (OCT_ATOM_TYPE_##AT) | ((OCT_DATA_TYPE_##DT)<<2) )

/* Combines the `oct_atomType` and `oct_dataType` enums into one for ocelot */
enum ocl_nodeType
{
    OCL_NODE_TYPE_BRANCH        = NODE_TYPE(CONTAINER, NONE),   /* Node containing a list of child nodes */
    OCL_NODE_TYPE_STRING        = NODE_TYPE(ELEMENT, STRING),   /* Node containing a single string element */
    OCL_NODE_TYPE_STRING_ARRAY  = NODE_TYPE(LIST, STRING),      /* Node containing a list of string elements */
    OCL_NODE_TYPE_FLOAT         = NODE_TYPE(ELEMENT, FLOAT),    /* Node containing a single float value */
    OCL_NODE_TYPE_FLOAT_ARRAY   = NODE_TYPE(LIST, FLOAT),       /* Node containing a list of float values */
    OCL_NODE_TYPE_INT           = NODE_TYPE(ELEMENT, INT),      /* Node containing a single integer value */
    OCL_NODE_TYPE_INT_ARRAY     = NODE_TYPE(LIST, INT),         /* Node containing a list of integer values */
    OCL_NODE_TYPE_BINARY        = NODE_TYPE(ELEMENT, BINARY),   /* Node containing a raw binary stream */
    /* Node containing a UUID value. NOTE: UUIDs are not explicitly part of the oct atom format. */
    OCL_NODE_TYPE_UUID          = NODE_TYPE(LIST, BINARY),
    OCL_NODE_TYPE_MAX           = 9
};

#undef NODE_TYPE

/* Implements the type of a scenes SceneTreeNodePool/Node atom */
enum oct_sceneNodeType
{
    OCT_SCENE_NODE_TYPE_NONE,               /* No type associated */
    OCT_SCENE_NODE_TYPE_SCENE,              /* Node represents an octane scene */
    OCT_SCENE_NODE_TYPE_TRANSFORM,          /* Node represents a scene world transform */
    OCT_SCENE_NODE_TYPE_GEOMETRY,           /* Node represents a scene model */
    OCT_SCENE_NODE_TYPE_SUB_GEOMETRY,       /* Node represents a scene models submesh */
    OCT_SCENE_NODE_TYPE_SUB_GEOMETRY_LIT,   /* Node represents a lit scene models submesh */
    OCT_SCENE_NODE_TYPE_CAMERA,             /* Node represents a scene camera */
    OCT_SCENE_NODE_TYPE_LIGHT,              /* Node represents a scene light */
    OCT_SCENE_NODE_TYPE_MAX
};

/* Implements a scene lights type */
enum oct_sceneLightType
{
    OCT_SCENE_LIGHT_TYPE_NONE,          /* No type associated */
    OCT_SCENE_LIGHT_TYPE_DIRECTIONAL,   /* Directional light */
    OCT_SCENE_LIGHT_TYPE_POINT,         /* Light emitting from a single point */
    OCT_SCENE_LIGHT_TYPE_AMBIENT,       /* Ambient lighting */
    OCT_SCENE_LIGHT_TYPE_MAX
};

enum oct_elementType
{
    OCT_ELEMENT_TYPE_FLOAT_ONE          = 0,    // R_FLOAT          | D3DDECLTYPE_FLOAT1    | DXGI_FORMAT_R32_FLOAT
    OCT_ELEMENT_TYPE_FLOAT_TWO          = 1,    // RG_FLOAT         | D3DDECLTYPE_FLOAT2    | DXGI_FORMAT_R32G32_FLOAT
    OCT_ELEMENT_TYPE_FLOAT_THREE        = 2,    // RGB_FLOAT        | D3DDECLTYPE_FLOAT3    | DXGI_FORMAT_R32G32B32_FLOAT
    OCT_ELEMENT_TYPE_FLOAT_FOUR         = 3,    // RGBA_FLOAT       | D3DDECLTYPE_FLOAT4    | DXGI_FORMAT_R32G32B32A32_FLOAT
    OCT_ELEMENT_TYPE_INT_ONE            = 10,   // R_SINT           | D3DDECLTYPE_INT1      | DXGI_FORMAT_R32_SINT
    OCT_ELEMENT_TYPE_SHORT_TWO_NORM     = 21,   // RG_HALF_NORM     | D3DDECLTYPE_SHORT2N   | DXGI_FORMAT_R16G16_SNORM
    OCT_ELEMENT_TYPE_SHORT_FOUR_NORM    = 23,   // RGBA_HALF_NORM   | D3DDECLTYPE_SHORT4N   | DXGI_FORMAT_R16G16B16A16_SNORM
    OCT_ELEMENT_TYPE_UBYTE_FOUR         = 24,   // RGBA_U8          | D3DDECLTYPE_UBYTE4    | DXGI_FORMAT_R8G8B8A8_UINT
    OCT_ELEMENT_TYPE_UBYTE_FOUR_NORM    = 25,   // RGBA_U8_NORM     | D3DDECLTYPE_UBYTE4N   | DXGI_FORMAT_R8G8B8A8_UNORM
    OCT_ELEMENT_TYPE_PVEC_THREE         = 39,   // S10_S11_S11_FLOAT| Not Supported         | DXGI_FORMAT_R11G11B10_FLOAT
    OCT_ELEMENT_TYPE_HALF_TWO           = 44,   // RG_HALF          | D3DDECLTYPE_FLOAT16_2 | DXGI_FORMAT_R16G16_FLOAT
    OCT_ELEMENT_TYPE_HALF_FOUR          = 45,   // RGBA_HALF        | D3DDECLTYPE_FLOAT16_4 | DXGI_FORMAT_R16G16B16A16_FLOAT
    OCT_ELEMENT_TYPE_UNUSED             = 84,   // void             | D3DDECLTYPE_UNUSED    | DXGI_FORMAT_UNKNOWN
};

enum ocl_vertexElementType
{
    OCL_VERTEX_ELEMENT_TYPE_NONE            = 0,
    OCL_VERTEX_ELEMENT_TYPE_POSITION        = 1<<0,
    OCL_VERTEX_ELEMENT_TYPE_UV1             = 1<<1,
    OCL_VERTEX_ELEMENT_TYPE_TANGENT         = 1<<2,
    OCL_VERTEX_ELEMENT_TYPE_NORMAL          = 1<<3,
    OCL_VERTEX_ELEMENT_TYPE_BINORMAL        = 1<<4,
    OCL_VERTEX_ELEMENT_TYPE_LIGHTMAP_UV     = 1<<5,
    OCL_VERTEX_ELEMENT_TYPE_COLOR_ADD       = 1<<6,
    OCL_VERTEX_ELEMENT_TYPE_COLOR_OCCL      = 1<<7,
    OCL_VERTEX_ELEMENT_TYPE_VERTEX_BAKED    = 1<<8,
    OCL_VERTEX_ELEMENT_TYPE_MAX             = 10
};

enum oct_primitiveType
{
    OCT_PRIMITIVE_TYPE_NONE,
    OCT_PRIMITIVE_TYPE_TRIANGLES,
    OCT_PRIMITIVE_TYPE_MAX
};
