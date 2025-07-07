#version 330 core
in vec3 v_normal;

out vec4 FragColor;


void main()
{
    vec3 light_dir = normalize(vec3(-0.5, -1.0, 0.2));
    vec3 norm = normalize(v_normal);
    //vec3 ambient = vec3(0.5);

    float diff = max(0.5, -dot(light_dir, norm));

    //FragColor = vec4(norm, 1.0);
    FragColor = vec4(vec3(diff), 1.0);
}
