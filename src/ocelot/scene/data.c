#include "ocelot/scene/data.h"
#include "data/dbuf.h"
#include "io/filepath.h"
#include "octane/ibuf/ibuf.h"
#include "octane/oct/atoms.h"
#include "octane/oct/oct.h"
#include "octane/oct/scene_descriptor.h"
#include "octane/vbuf/vbuf.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static void _load_vbuf_file(ocl_sceneData* const data)
{

    oct_sceneDescriptor* desc = data->scene_descriptor;
    oct_file* oct = data->file_data;

    oct_vertexBufferAtom* vba = NULL;

    //Keep looking until we found a vertex buffer with a file name
    for(uint32_t i = 0; i < desc->vbuf_pool_size && vba == NULL; i++)
    {
        oct_vertexBufferAtom atom = desc->vbuf_pool[i];

        if(atom.file_name != 0)
            vba = &desc->vbuf_pool[i];
    }

    if(vba == NULL)
    {
        fprintf(stderr, "[Ocelot]: Failed to find the .vbuf file.\n");
        return;
    }

    filepath_t world_root = data->file_data->world_path;
    filepath_t atom_path = fs_path_create(oct->string_table[vba->file_name].data);
    filepath_t vbuf_path = fs_path_combine(world_root, atom_path);

    data->vbuf_file = dbuf_load(vbuf_path.path);
}

static void _load_ibuf_file(ocl_sceneData* const data)
{
    oct_sceneDescriptor* desc = data->scene_descriptor;
    oct_file* oct = data->file_data;

    oct_indexBufferAtom* iba = NULL;

    //Keep looking until we found an index buffer with a file name
    for(uint32_t i = 0; i < desc->ibuf_pool_size && iba == NULL; i++)
    {
        oct_indexBufferAtom atom = desc->ibuf_pool[i];

        if(atom.file_name != 0)
            iba = &desc->ibuf_pool[i];
    }

    if(iba == NULL)
    {
        fprintf(stderr, "[Ocelot]: Failed to fine the .ibuf file.\n");
        return;
    }

    filepath_t world_root = data->file_data->world_path;
    filepath_t atom_path = fs_path_create(oct->string_table[iba->file_name].data);
    filepath_t ibuf_path = fs_path_combine(world_root, atom_path);

    data->ibuf_file = dbuf_load(ibuf_path.path);
    data->ibuf_stride = iba->width;
}

static void _load_index_buffers(ocl_sceneData* const data)
{
    oct_sceneDescriptor* desc = data->scene_descriptor;

    data->index_buffer_count = desc->istream_pool_size;
    data->index_buffers = calloc(data->index_buffer_count, sizeof(oct_indexBuffer));

    //printf("[ocelot]: Loading %u index buffers\n", data->index_buffer_count);

    for(uint32_t i = 0; i < data->index_buffer_count; i++)
    {
        data->index_buffers[i] = oct_load_index_buffer(&data->ibuf_file, *desc, data->ibuf_stride, i);
    }

    printf("[ocelot]: Loaded %u index buffers.\n", data->index_buffer_count);
}

static void _load_vertex_buffers(ocl_sceneData* const data)
{
    oct_sceneDescriptor* desc = data->scene_descriptor;

    data->vertex_buffer_count = desc->vstream_pool_size;
    data->vertex_buffers = calloc(data->vertex_buffer_count, sizeof(oct_vertexBuffer));

    for(uint32_t i = 0; i < data->vertex_buffer_count; i++)
    {
        data->vertex_buffers[i] = oct_load_vertex_buffer(&data->vbuf_file, *desc, i);
    }

    printf("[ocelot]: Loaded %u vertex buffers.\n", data->vertex_buffer_count);
}

ocl_sceneData ocl_load_scene_data(oct_sceneDescriptor* const scene, oct_file* const oct)
{

    ocl_sceneData data = (ocl_sceneData){0};

    //Link the external references
    data.scene_descriptor = scene;
    data.file_data = oct;

    //Load the vertex and index buffer files
    _load_vbuf_file(&data);
    _load_ibuf_file(&data);

    _load_index_buffers(&data);
    _load_vertex_buffers(&data);

    return data;
}

void ocl_free_scene_data(ocl_sceneData* const scene)
{
    if(scene != NULL)
    {
        dbuf_free(&scene->vbuf_file);
        dbuf_free(&scene->ibuf_file);

        for(uint32_t i = 0; i < scene->index_buffer_count; i++)
        {
            oct_free_index_buffer(&scene->index_buffers[i]);
        }
        free(scene->index_buffers);

        for(uint32_t i = 0; i < scene->vertex_buffer_count; i++)
        {
            oct_free_vertex_buffer(&scene->vertex_buffers[i]);
        }
        free(scene->vertex_buffers);

    }
}
