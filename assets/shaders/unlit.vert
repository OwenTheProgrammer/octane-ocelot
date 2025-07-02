#version 330 core
layout (location = 0) in vec3 vpos;

uniform mat4 view;
uniform mat4 proj;

void main()
{
    gl_Position = proj * view * vec4(vpos, 1.0);
    //gl_Position = vec4(pos, 1.0);
}
