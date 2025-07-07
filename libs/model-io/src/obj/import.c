#include "model/obj.h"
#include "internal.h"
#include "console/log.h"
#include "obj/obj_internal.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _READ_LINE_MAX 1024

static _elemTag _get_line_type(char* const line)
{
    switch(line[0]) {
        case '#':   return _ELEM_TAG_COMMENT;
        case 'o':   return _ELEM_TAG_NAME;
        case 's':   return _ELEM_TAG_SHADING;
        case 'f':   return _ELEM_TAG_FACE_ELEM;
        case 'v':
            switch(line[1])
            {
                case ' ':   return _ELEM_TAG_POSITION;
                case 'n':   return _ELEM_TAG_NORMAL;
                case 't':   return _ELEM_TAG_TEXCOORD;
                default:
                    break;
            }
            break;

        default:
            break;
    }
    return _ELEM_TAG_MAX;
}

static size_t _get_char_count(char target, char* data, size_t size)
{
    size_t count = 0;

    for(size_t i = 0; i < size; i++)
    {
        count += data[i] == target;
    }

    return count;
}

static void _read_float_list(char* data, float* const dst, size_t count)
{
    for(size_t i = 0; i < count; i++)
    {
        dst[i] = strtof(data, &data);
        data++;
    }
}

static _faceElement _read_element(char** ptr)
{
    uint32_t values[3] = {0,0,0};

    char* _ptr = *ptr;

    for(uint32_t i = 0; i < 3; i++)
    {
        // Skip the element if it doesnt exist
        if(*_ptr == '/')
        {
            _ptr++;
            continue;
        }

        // Read the value
        values[i] = strtoul(_ptr, &_ptr, 10);

        // Exit if the next value is an ending term
        if(*_ptr == ' ' || *_ptr == '\n')
            break;

        // Move one character over
        _ptr++;
    }

    // Update the movement
    *ptr = _ptr;

    return (_faceElement)
    {
        .v_index = values[0],
        .t_index = values[1],
        .n_index = values[2]
    };
}

static _face _read_face(char* data, size_t space_count)
{
    _face f = (_face){0};

    f.element_count = space_count;
    f.elements = calloc(space_count, sizeof(_faceElement));

    for(size_t i = 0; i < space_count; i++)
    {
        f.elements[i] = _read_element(&data);
    }

    return f;
}

static int _face_element_eq(_faceElement a, _faceElement b)
{
    return (a.v_index == b.v_index) && (a.t_index == b.t_index) && (a.n_index == b.n_index);
}

static uint32_t _find_element_in_table(_faceElement* const table, size_t table_size, _faceElement elem)
{
    for(size_t i = 0; i < table_size; i++)
    {
        if(_face_element_eq(table[i], elem))
            return (uint32_t)i;
    }
    return UINT32_MAX;
}

static _faceElement* _get_unique_elements(_objData data, size_t* const unique_count)
{
    _faceElement* unique_table = calloc(data.face_count * 4, sizeof(_faceElement));
    size_t count = 0;

    for(size_t i = 0; i < data.face_count; i++)
    {
        _face face = data.face[i];
        for(size_t j = 0; j < face.element_count; j++)
        {
            _faceElement elem = face.elements[j];

            uint32_t idx = _find_element_in_table(unique_table, count, elem);

            // Add the element to the table
            if(idx == UINT32_MAX)
                unique_table[count++] = elem;
        }
    }
    unique_table = realloc(unique_table, count * sizeof(_faceElement));

    *unique_count = count;

    return unique_table;
}

static char* _read_file(const char* filepath, size_t* const restrict read_size, size_t* const restrict read_lines)
{
    if(filepath == NULL)
    {
        LOG_ERROR("Filepath can not be null.\n");
        return NULL;
    }

    FILE* file = fopen(filepath, "r");

    if(file == NULL)
    {
        LOG_ERROR("Failed to load " OCL_FILEPATH_FMT "\n", filepath);
        return NULL;
    }

    // Get the file size
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Read the files data
    char* file_data = calloc(file_size+1, sizeof(char));
    fread(file_data, sizeof(char), file_size, file);
    fclose(file);

    size_t line_count = _get_char_count('\n', file_data, file_size);
    LOG_INFO("Reading %zu lines from " OCL_FILEPATH_FMT "\n", line_count, filepath);

    *read_size = file_size;
    *read_lines = line_count;

    return file_data;
}

static _objData _read_obj_file(const char* filepath)
{
    size_t data_size = 0;
    size_t line_count = 0;
    char* data = _read_file(filepath, &data_size, &line_count);

    struct lineFeed
    {
        _elemTag tag;
        char* ptr;
        size_t line_len;
        size_t space_count;
    };

    struct lineFeed* lines = calloc(line_count, sizeof(struct lineFeed));

    struct attribCounter
    {
        size_t pos;
        size_t normal;
        size_t texcoord;
        size_t face;
    };

    struct attribCounter counter = (struct attribCounter){0};

    // Serialize the lines of the file
    char* nl_ptr = data;
    for(size_t i = 0; i < line_count; i++)
    {
        lines[i].tag = _get_line_type(nl_ptr);
        lines[i].ptr = nl_ptr;

        switch(lines[i].tag)
        {
            case _ELEM_TAG_POSITION:    counter.pos++; break;
            case _ELEM_TAG_NORMAL:      counter.normal++; break;
            case _ELEM_TAG_TEXCOORD:    counter.texcoord++; break;
            case _ELEM_TAG_FACE_ELEM:   counter.face++; break;
            default: break;
        }

        // Find the next newline
        nl_ptr = strchr(nl_ptr, '\n');
        if(nl_ptr == NULL)
            nl_ptr = data + (data_size-1);

        lines[i].line_len = (size_t)(nl_ptr - lines[i].ptr);
        lines[i].space_count = _get_char_count(' ', lines[i].ptr, lines[i].line_len);
        nl_ptr++;
    }


    _objData result = (_objData){0};

    result.pos = calloc(counter.pos * 3, sizeof(float));
    result.norm = calloc(counter.normal * 3, sizeof(float));
    result.texcoord = calloc(counter.texcoord * 2, sizeof(float));
    result.face = calloc(counter.face, sizeof(_face));

    // Parse the serialized data
    for(size_t i = 0; i < line_count; i++)
    {
        struct lineFeed lf = lines[i];
        switch(lf.tag)
        {
            case _ELEM_TAG_NAME:
                result.name = calloc(lf.line_len-1, sizeof(char));
                memcpy(result.name, lf.ptr + 2, lf.line_len-2);
                result.name_len = lf.line_len - 2;
                break;

            case _ELEM_TAG_POSITION:
                _read_float_list(lf.ptr + 2, &result.pos[result.pos_count * 3], 3);
                result.pos_count++;
                break;

            case _ELEM_TAG_TEXCOORD:
                _read_float_list(lf.ptr + 3, &result.texcoord[result.texcoord_count * 2], 2);
                result.texcoord_count++;
                break;

            case _ELEM_TAG_NORMAL:
                _read_float_list(lf.ptr + 3, &result.norm[result.norm_count * 3], 3);
                result.norm_count++;
                break;

            case _ELEM_TAG_FACE_ELEM:
                result.face[result.face_count] = _read_face(lf.ptr + 2, lf.space_count);
                result.element_count += result.face[result.face_count].element_count;
                result.face_count++;
                break;

            default:
                break;
        }
    }

    free(lines);
    free(data);

    return result;
}

mdl_obj mdl_obj_load_file(const char* filepath)
{
    _objData data = _read_obj_file(filepath);

    int has_positions = data.pos_count != 0;
    int has_normals = data.norm_count != 0;
    int has_texcoords = data.texcoord_count != 0;

    // Find all the unique face elements
    size_t unique_count = 0;
    _faceElement* unique_elems = _get_unique_elements(data, &unique_count);

    mdl_obj obj = (mdl_obj){0};

    // Copy the name
    obj.name = calloc(data.name_len+1, sizeof(char));
    memcpy(obj.name, data.name, data.name_len);


    // Build the index buffer

    // WARN: This requires the mesh to be triangulated
    obj.indices = calloc(data.element_count, sizeof(uint32_t));

    for(size_t i = 0; i < data.face_count; i++)
    {
        _face face = data.face[i];
        for(size_t j = 0; j < face.element_count; j++)
        {
            _faceElement elem = face.elements[j];

            // Link the face element to the unique table index
            obj.indices[obj.index_count] = _find_element_in_table(unique_elems, unique_count, elem);
            obj.index_count++;
        }
    }

    // Build the vertex buffer
    obj.vertex_count = unique_count;

    if(has_positions)
        obj.positions = calloc(unique_count * 3, sizeof(float));
    if(has_normals)
        obj.normals = calloc(unique_count * 3, sizeof(float));
    if(has_texcoords)
        obj.texcoords = calloc(unique_count * 2, sizeof(float));

    for(size_t i = 0; i < unique_count; i++)
    {
        _faceElement elem = unique_elems[i];

        // Copy the face elements indexed values
        // NOTE: All elements are 1 base indexed for some reason
        if(has_positions)
            memcpy(&obj.positions[i * 3], &data.pos[(elem.v_index-1) * 3], sizeof(float) * 3);

        if(has_normals)
            memcpy(&obj.normals[i * 3], &data.norm[(elem.n_index-1) * 3], sizeof(float) * 3);

        if(has_texcoords)
            memcpy(&obj.texcoords[i * 2], &data.texcoord[(elem.t_index-1) * 2], sizeof(float) * 2);
    }


    free(unique_elems);
    _free_obj_data(&data);

    LOG_INFO("Loaded \"%s\" successfully. (verts: %zu | tris: %zu)\n",
             obj.name, obj.vertex_count, obj.index_count/3
    );

    return obj;
}
