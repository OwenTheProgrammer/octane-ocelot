#include "obj/obj_internal.h"
#include "model/obj.h"
#include <stdint.h>
#include <stdlib.h>

const char* _ELEM_TAG_TABLE[_ELEM_TAG_MAX] =
{
    "#",
    "o",
    "v",
    "vn",
    "vt",
    "s",
    "f",
};


void _free_obj_data(_objData* const data)
{
    free(data->name);

    free(data->pos);

    free(data->norm);

    free(data->texcoord);

    for(uint32_t i = 0; i < data->face_count; i++)
    {
        free(data->face[i].elements);
    }

    free(data->face);
}


void mdl_free_obj(mdl_obj* const obj)
{
    free(obj->name);
    free(obj->positions);
    free(obj->normals);
    free(obj->texcoords);
    free(obj->indices);
}
