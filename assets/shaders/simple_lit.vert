#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;

out vec3 v_normal;

uniform mat4 view;
uniform mat4 proj;

void main()
{
    gl_Position = proj * view * vec4(pos, 1.0);
    v_normal = normal;
}
