#version 330 core

layout(location = 0) in vec3 vpos;
layout(location = 1) in vec2 vcoord;

uniform mat4 view;
uniform mat4 proj;

out vec2 uv;

void main()
{
    gl_Position = proj * view * vec4(vpos, 1.0);
    uv = vcoord;
}
