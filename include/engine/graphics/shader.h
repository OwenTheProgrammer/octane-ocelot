#pragma once

#include "octane/vbuf/vbuf.h"
#include <glad/glad.h>

typedef struct oce_shader   oce_shader;

struct oce_shader
{
    GLuint program;
};

/* Loads the vertex and fragment stages of a shader from the ocelot engine assets */
oce_shader oce_shader_load_vf(const char* shader_name);

/* Loads a vertex buffer as an OpenGL buffer */
GLuint oce_shader_load_vertex_buffer(oct_vertexBuffer buffer);

/* Disposes the contents of a shader */
void oce_shader_free(oce_shader* const shader);
