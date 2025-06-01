#pragma once
#include "octane/oct.h"
#include <stdint.h>

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
};

extern oct_atomNameTable _oct_ant;

/* Serialization of IndexStreamPool/IndexStream */
struct oct_indexStreamAtom
{
    uint16_t name_key;

    uint32_t length;

    uint32_t buffer_reference;

    uint32_t buffer_offset;

    oct_primitiveType stream_primitive;
};

/* Serialization of VertexStreamPool/VertexStream */
struct oct_vertexStreamAtom
{
    uint16_t node_name;

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
    uint16_t node_name;

    oct_elementType type;

    uint16_t name;

    oct_vstreamElementName attribute_type;

    uint32_t offset;
};


/* Returns the string table index of a string, 0 if not found */
uint16_t oct_get_st_index_of(oct_file oct, const char* str);

uint32_t oct_get_atom_node_index(oct_file oct, uint16_t key);


oct_atomNameTable oct_create_atom_name_table(oct_file oct);

void oct_init_atom_name_table(oct_file oct);

void oct_print_atom_name_table(oct_file oct, oct_atomNameTable ant);


void oct_print_atom_index_stream(oct_file oct, oct_indexStreamAtom atom);

void oct_print_atom_vertex_stream(oct_file oct, oct_vertexStreamAtom atom);

void oct_print_atom_vertex_element(oct_file oct, oct_vstreamElementAtom atom);
