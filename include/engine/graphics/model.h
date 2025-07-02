#pragma once

#include "engine/graphics/shader.h"
#include "ocelot/scene/data.h"
#include "octane/oct/enums.h"
#include <glad/glad.h>
#include <stdint.h>

typedef struct oce_vertexAttrib oce_vertexAttrib;

typedef struct oce_modelElement oce_modelElement;
typedef struct oce_model        oce_model;

struct oce_vertexAttrib
{
    ocl_vertexElementType type;

    uint32_t offset;

    uint32_t stride;
};

struct oce_modelElement
{
    uint32_t count;

    uint32_t stride;

    GLuint gl_buffer;
};

struct oce_model
{
    /* The length of `stream_table` */
    uint32_t stream_count;

    /* Stored as [INDEX_STREAM_REF, VERTEX_STREAM_REF[]]*/
    uint32_t* stream_table;

    ocl_vertexElementFlag attribute_flags;

    oce_vertexAttrib attributes[OCL_VERTEX_ELEMENT_TYPE_MAX];

    GLuint gl_vertArray;

    oce_modelElement vertex;

    oce_modelElement index;

    void* vbuf_data;

    oce_shader shader;
};


void oce_model_load_streams(oce_model* const model, ocl_sceneData* const data);


void oce_model_load_oct_index_buffer(oce_model* const target, ocl_sceneData* const data, uint32_t stream_index);

void oce_model_load_oct_vertex_buffer(oce_model* const target, ocl_sceneData* const data);

void oce_model_gen_va(oce_model* const model);


void oce_model_load_shader(oce_model* const target, oce_shader shader);


void oce_model_draw(oce_model model);

void oce_free_model(oce_model* const model);
