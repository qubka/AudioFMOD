#version 330 core

layout (location = 0) out vec4 o_color;

in vec3 v_tex_coord;

uniform samplerCube skybox;

void main()
{
    o_color = texture(skybox, v_tex_coord);
}