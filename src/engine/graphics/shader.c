#include "engine/graphics/shader.h"
#include "data/dbuf.h"
#include "engine/core/runtime.h"
#include "io/filepath.h"
#include "octane/vbuf/vbuf.h"
#include "engine/internal.h"
#include "console/log.h"
#include <alloca.h>
#include <stdint.h>
#include <stdio.h>

static GLuint _compile_shader_stage(filepath_t path, GLuint stage)
{
    dbuf file = dbuf_load(path.path);
    dbuf_ensure_capacity(&file, file.size+1);

    if(!dbuf_is_valid(&file))
    {
        LOG_FATAL("Failed to load shader " OCL_FILEPATH_FMT "\n", path.path);
        //fprintf(stderr, "[Engine]: Failed to load shader \"%s\"\n", path.path);
        return -1;
    }

    const char* sv_ptr = file.data;

    GLuint shader = glCreateShader(stage);
    glShaderSource(shader, 1, &sv_ptr, NULL);
    glCompileShader(shader);

    dbuf_free(&file);

    GLint compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if(compiled == GL_FALSE)
    {
        GLint maxlen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxlen);

        GLchar* log = alloca(maxlen * sizeof(GLchar));
        glGetShaderInfoLog(shader, maxlen, &maxlen, log);

        LOG_API_FATAL("openGL", "Error compiling " OCL_FILEPATH_FMT "\n\t\"%s\"\n", path.path, log);
        //fprintf(stderr, "[OpenGL]: Error compiling \"%s\"\n\t\"%s\"\n", path.path, log);

        glDeleteShader(shader);
        return -1;
    }

    return shader;
}

static GLuint _link_shader_stages(GLuint vert, GLuint frag)
{
    GLuint program = glCreateProgram();

    glAttachShader(program, vert);
    glAttachShader(program, frag);

    glLinkProgram(program);

    GLint linked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if(linked == GL_FALSE)
    {
        GLint maxlen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxlen);

        GLchar* log = alloca(maxlen * sizeof(GLchar));
        glGetProgramInfoLog(program, maxlen, &maxlen, log);

        LOG_API_FATAL("openGL", "Linker error.\n\t%s\n", log);
        //fprintf(stderr, "[OpenGL]: Linker error.\n\t%s\n", log);

        glDeleteProgram(program);
        glDeleteShader(vert);
        glDeleteShader(frag);

        return -1;
    }

    GLint valid = 0;
    glValidateProgram(program);
    glGetProgramiv(program, GL_VALIDATE_STATUS, &valid);
    if(valid == GL_FALSE)
    {
        GLint maxlen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxlen);

        GLchar* log = alloca(maxlen * sizeof(GLchar));
        glGetProgramInfoLog(program, maxlen, &maxlen, log);

        LOG_API_FATAL("openGL", "Invalid shader program.\n\t%s\n", log);
        //fprintf(stderr, "[OpenGL]: Invalid shader program.\n\t%s\n", log);

        // TODO: Create a backup shader for this failure case

        glDetachShader(program, vert);
        glDetachShader(program, frag);

        glDeleteProgram(program);
        glDeleteShader(vert);
        glDeleteShader(frag);

        return -1;
    }

    glDetachShader(program, vert);
    glDetachShader(program, frag);

    glDeleteShader(vert);
    glDeleteShader(frag);

    return program;
}

oce_shader oce_shader_load_vf(const char* shader_name)
{
    filepath_t vert_path = fs_path_concat(_rt.path.assets_dir, "shaders/%s.vert", shader_name);
    filepath_t frag_path = fs_path_concat(_rt.path.assets_dir, "shaders/%s.frag", shader_name);

    oce_shader shader = (oce_shader){0};

    GLuint vert_id = _compile_shader_stage(vert_path, GL_VERTEX_SHADER);
    if(vert_id == -1)
        return shader;

    GLuint frag_id = _compile_shader_stage(frag_path, GL_FRAGMENT_SHADER);
    if(frag_id == -1)
    {
        glDeleteShader(vert_id);
        return shader;
    }

    shader.program = _link_shader_stages(vert_id, frag_id);

    LOG_INFO("Loaded shader " OCL_FILEPATH_FMT "\n", shader_name);
    //printf("[Engine]: Loaded shader \"%s\"\n", shader_name);

    return shader;
}


GLuint oce_shader_load_vertex_buffer(oct_vertexBuffer buffer)
{
    GLuint vbo;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        buffer.vertex_count * sizeof(oct_vertex),
        buffer.vertices,
        GL_STATIC_DRAW
    );

    return vbo;
}

void oce_shader_free(oce_shader* const shader)
{
    glDeleteProgram(shader->program);
}
