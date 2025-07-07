#pragma once

#include <stdint.h>
#ifndef __cplusplus

typedef enum _elemTag   _elemTag;

#endif

typedef struct _face        _face;
typedef struct _faceElement _faceElement;
typedef struct _objData     _objData;

enum _elemTag
{
    _ELEM_TAG_COMMENT,
    _ELEM_TAG_NAME,
    _ELEM_TAG_POSITION,
    _ELEM_TAG_NORMAL,
    _ELEM_TAG_TEXCOORD,
    _ELEM_TAG_SHADING,
    _ELEM_TAG_FACE_ELEM,
    _ELEM_TAG_MAX
};

extern const char* _ELEM_TAG_TABLE[_ELEM_TAG_MAX];

struct _faceElement
{
    uint32_t v_index;

    uint32_t t_index;

    uint32_t n_index;
};

struct _face
{
    uint32_t element_count;

    _faceElement* elements;
};

struct _objData
{
    char* name;

    uint32_t name_len;


    uint32_t pos_count;

    float* pos;


    uint32_t norm_count;

    float* norm;


    uint32_t texcoord_count;

    float* texcoord;


    uint32_t face_count;

    uint32_t element_count;

    _face* face;
};

void _free_obj_data(_objData* const data);
