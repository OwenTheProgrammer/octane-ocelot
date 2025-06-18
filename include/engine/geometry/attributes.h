/* Defines the possible attributes for a vertex buffer stream */
#pragma once
#include "octane/oct_atoms.h"
#include <stdint.h>

typedef struct oce_vertexAttributes     oce_vertexAttributes;
typedef struct oce_vattrib_position     oce_vattrib_position;

/* Model vertex position attribute */
struct oce_vattrib_position
{
    /* Attribute offset from base pointer in vertex stream */
    uint32_t offset;

    /* Raw vertex position buffer */
    float* buffer;
};


/* Holds all the vertex attribute buffers in one struct before consolidation */
struct oce_vertexAttributes
{
    /* Flags enum representing which attributes are stored */
    oct_vstreamElementName flags;

    oce_vattrib_position position;
    
};