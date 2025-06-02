#include "ocelot/engine/window.h"
#include "ocelot/engine/model.h"
#include "ocelot/engine_state.h"
#include "types/vector.h"

#include <GL/glext.h>
#include <GLFW/glfw3.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static void error_callback(int error, const char* msg)
{
    fprintf(stderr, "GLFW: error %i \"%s\"\n", error, msg);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void fb_resize_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


int ocl_gui_init()
{
    glfwSetErrorCallback(error_callback);

    if(!glfwInit())
        return 0;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    ocl_rtState.window = glfwCreateWindow(1280, 720, "Ocelot", NULL, NULL);

    if(!ocl_rtState.window)
    {
        glfwTerminate();
        return 0;
    }


    glfwMakeContextCurrent(ocl_rtState.window);

    glfwSetKeyCallback(ocl_rtState.window, key_callback);
    glfwSetFramebufferSizeCallback(ocl_rtState.window, fb_resize_callback);

    //Vsync
    glfwSwapInterval(1);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fprintf(stderr, "Failed to initialize glad.\n");
        glfwDestroyWindow(ocl_rtState.window);
        glfwTerminate();
        return 0;
    }


    return 1;
}

void ocl_gui_loop(oce_model model)
{

    for(uint32_t i = 0; i < model.vertex_count; i++)
    {
        vec3f pos = model.vertex_positions[i];
        printf("%u| %.4f %.4f, %.4f\n", i, pos.x, pos.y, pos.z);
    }

    FILE* shader_vert = fopen("bin/assets/shaders/unlit.vert", "rb");
    if(shader_vert == NULL)
    {
        fprintf(stderr, "Failed to open shader file.\n");
    }

    fseek(shader_vert, 0, SEEK_END);
    size_t vert_size = ftell(shader_vert);
    fseek(shader_vert, 0, SEEK_SET);

    char* shader_vert_data = calloc(vert_size, sizeof(char));
    fread(shader_vert_data, sizeof(char), vert_size+1, shader_vert);
    fclose(shader_vert);

    const char* sv_ptr = shader_vert_data;

    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &sv_ptr, NULL);
    glCompileShader(vertex_shader);
    free(shader_vert_data);


    shader_vert = fopen("bin/assets/shaders/unlit.frag", "rb");
    if(shader_vert == NULL)
    {
        fprintf(stderr, "Failed to open shader file.\n");
    }

    fseek(shader_vert, 0, SEEK_END);
    vert_size = ftell(shader_vert);
    fseek(shader_vert, 0, SEEK_SET);

    shader_vert_data = calloc(vert_size, sizeof(char));
    fread(shader_vert_data, sizeof(char), vert_size+1, shader_vert);
    fclose(shader_vert);

    sv_ptr = shader_vert_data;

    unsigned int frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &sv_ptr, NULL);
    glCompileShader(frag_shader);
    free(shader_vert_data);

    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, frag_shader);
    glLinkProgram(shader_program);

    //glUseProgram(shader_program);

    glDeleteShader(vertex_shader);
    glDeleteShader(frag_shader);

    unsigned int vbo, vao, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);


    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3f) * model.vertex_count, model.vertex_positions, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * model.index_count, model.indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3f), NULL);
    glEnableVertexAttribArray(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while(!glfwWindowShouldClose(ocl_rtState.window))
    {

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, model.index_count, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(ocl_rtState.window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteProgram(shader_program);

    glfwDestroyWindow(ocl_rtState.window);
    glfwTerminate();
}

