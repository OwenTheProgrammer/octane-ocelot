#pragma once

#include <stdint.h>
typedef struct mdl_obj  mdl_obj;

struct mdl_obj
{
    char* name;

    uint32_t vertex_count;

    float* positions;

    float* normals;

    float* texcoords;

    uint32_t index_count;

    uint32_t* indices;
};

mdl_obj mdl_obj_load_file(const char* filepath);

void mdl_free_obj(mdl_obj* const obj);
