#include "engine/graphics/model.h"
#include "engine/internal.h"
#include "engine/graphics/internal.h"
#include "ocelot/scene/data.h"
#include "octane/ibuf/ibuf.h"
#include "octane/oct/enums.h"
#include "octane/vbuf/vbuf.h"
#include <stddef.h>
#include <stdint.h>
#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const ocl_vertexElementFlag _ELEMENT_TYPE_SUPPORT = OCL_VERTEX_ELEMENT_FLAG_NONE
    | OCL_VERTEX_ELEMENT_FLAG_POSITION
//    | OCL_VERTEX_ELEMENT_FLAG_UV1
;

void oce_model_load_streams(oce_model* const model, ocl_sceneData* const data)
{
    oce_model_load_oct_index_buffer(model, data, model->stream_table[0]);
    oce_model_load_oct_vertex_buffer(model, data);
    oce_model_gen_va(model);

    LOG_INFO("Loaded model %u with %u triangles (%u)\n",
             model->gl_vertArray,
             model->index.count/3,
             model->attribute_flags);
    //printf("[engine]: Loaded model %u with %u triangles (%u)\n",
    //       model->gl_vertArray,
    //       model->index.count/3,
    //       model->attribute_flags
    //);
}


void oce_model_load_oct_index_buffer(oce_model* const target, ocl_sceneData* const data, uint32_t stream_index)
{
    oce_modelElement element = (oce_modelElement){0};

    oct_indexBuffer buffer = data->index_buffers[stream_index];

    element.count = buffer.index_count;
    //element.stride = data->ibuf_stride;
    element.stride = sizeof(uint32_t);

    if(element.count != 0 && buffer.indices != NULL)
    {
        // Create the element buffer for the index data
        glGenBuffers(1, &element.gl_buffer);

        // Bind the element buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element.gl_buffer);

        // Copy the buffer data
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            element.count * element.stride,
            buffer.indices,
            GL_STATIC_DRAW
        );

        // Unbind the element buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    target->index = element;
}

void oce_model_load_oct_vertex_buffer(oce_model* const target, ocl_sceneData* const data)
{

    if(target->stream_count <= 1)
        return;

    struct attribute
    {
        void* src_ptr;

        uint32_t dst_offset;

        uint32_t elem_stride;
    };

    uint32_t attr_count = 0;
    struct attribute attr_stack[OCL_VERTEX_ELEMENT_TYPE_MAX];

    oce_modelElement element = (oce_modelElement){0};

    // Precompute the stride, skip the index stream with i=1
    for(uint32_t i = 1; i < target->stream_count; i++)
    {
        uint32_t stream_ref = target->stream_table[i];
        oct_vertexBuffer vbuf = data->vertex_buffers[stream_ref];

        ocl_vertexElementFlag supported = vbuf.element_flags & _ELEMENT_TYPE_SUPPORT;

        if(target->attribute_flags & supported)
        {
            LOG_WARN("Overlapping attributes found in vertex buffer.\n");
            //fprintf(stderr, "[engine]: Overlapping attributes found in vertex buffer.\n");
            return;
        }

        // Leave early if we didnt do anything
        if(supported == OCL_VERTEX_ELEMENT_FLAG_NONE)
            continue;

        element.count = vbuf.vertex_count;

        // Scan through all element types and add the ones that are present
        for(uint32_t j = 0; j < OCL_VERTEX_ELEMENT_TYPE_MAX; j++)
        {
            ocl_vertexElementFlag target_type = (ocl_vertexElementFlag)(1 << j);
            //Check if the current buffer has the target flag
            if(supported & target_type)
            {
                //Set the pointer to the first attribute of this type
                uint32_t attr_offset = _ATTRIBUTE_INFO[j].src_offset;
                attr_stack[attr_count].src_ptr = vbuf.vertices + attr_offset;

                //Add the element to the stride table
                attr_stack[attr_count].dst_offset = element.stride;
                element.stride += _ATTRIBUTE_INFO[j].stride;

                //Add the element stride
                attr_stack[attr_count].elem_stride = _ATTRIBUTE_INFO[j].stride;

                //Submit to the attributes table
                oce_vertexAttrib* attrib = &target->attributes[j];
                attrib->type = (ocl_vertexElementType)(j+1);
                attrib->offset = attr_stack[attr_count].dst_offset;
                attrib->stride = attr_stack[attr_count].elem_stride;

                //Increment the attribute stack counter
                attr_count++;
            }
        }

        target->attribute_flags |= supported;
    }
    element.stride = oct_get_vertex_buffer_stride(target->attribute_flags);

    if(element.stride == 0 || element.count == 0)
        return;

    LOG_INFO("Element: %u count with %u stride\n", element.count, element.stride);
    //printf(" Element: %u count with %u stride\n", element.count, element.stride);

    //Construct the buffer to present to OpenGL
    char* data_buf = calloc(element.count * element.stride, sizeof(char));

    // Write each element
    for(uint32_t i = 0; i < attr_count; i++)
    {
        struct attribute attr = attr_stack[i];

        char* src_base_ptr = (char*)attr.src_ptr;
        char* dst_base_ptr = data_buf + attr.dst_offset;

        // Write all the vertex elements of the current type
        for(uint32_t j = 0; j < element.count; j++)
        {
            memcpy(
                dst_base_ptr + j * element.stride,
                src_base_ptr + j * sizeof(oct_vertex),
                attr.elem_stride
            );
        }
    }

    // Generate a new OpenGL buffer for the vertex stream
    glGenBuffers(1, &element.gl_buffer);

    // Bind the vertex stream buffer
    glBindBuffer(GL_ARRAY_BUFFER, element.gl_buffer);

    // Copy the combined buffer data into the OpenGL vertex stream
    glBufferData(
        GL_ARRAY_BUFFER,
        element.count * element.stride,
        data_buf,
        GL_STATIC_DRAW
    );

    // Unbind the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //free(data_buf);
    target->vbuf_data = data_buf;

    target->vertex = element;
}

void oce_model_gen_va(oce_model* const model)
{
    if(model->attribute_flags == OCL_VERTEX_ELEMENT_FLAG_NONE)
        return;

    // Generate the vertex array object (VAO)
    glGenVertexArrays(1, &model->gl_vertArray);

    glBindVertexArray(model->gl_vertArray);                         // Bind VAO
    glBindBuffer(GL_ARRAY_BUFFER, model->vertex.gl_buffer);         // Bind VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->index.gl_buffer);  // Bind EBO

    // Enable the vertex attributes that exist
    for(uint32_t i = 0; i < OCL_VERTEX_ELEMENT_FLAG_MAX; i++)
    {
        ocl_vertexElementFlag target = (ocl_vertexElementFlag)(1 << i);

        if(model->attribute_flags & target)
        {
            struct _attribInfo info = _ATTRIBUTE_INFO[i];

            // We found a flag thats present
            // Define the layout
            glVertexAttribPointer(
                info.layout_location,
                info.component_count,
                info.component_type,
                GL_FALSE,
                info.stride,
                (void*)((size_t)info.src_offset)
            );
            // Enable the attribute
            glEnableVertexAttribArray(info.layout_location);
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);   // Unbind VBO
    glBindVertexArray(0);               // Unbind VAO
}


void oce_model_load_shader(oce_model* const target, oce_shader shader)
{
    target->shader = shader;
}

void oce_model_draw(oce_model model)
{
    if(model.gl_vertArray != 0 && model.vertex.gl_buffer != 0 && model.index.gl_buffer != 0)
    {
        glUseProgram(model.shader.program);
        glBindVertexArray(model.gl_vertArray);
        glDrawElements(GL_TRIANGLES, model.index.count, GL_UNSIGNED_INT, 0);
    }
}


void oce_free_model(oce_model* const model)
{
    if(model != NULL)
    {
        free(model->stream_table);

        if(model->gl_vertArray != 0)
            glDeleteVertexArrays(1, &model->gl_vertArray);

        if(model->vertex.gl_buffer)
            glDeleteBuffers(1, &model->vertex.gl_buffer);

        if(model->index.gl_buffer)
            glDeleteBuffers(1, &model->index.gl_buffer);

        free(model->vbuf_data);
    }
}
