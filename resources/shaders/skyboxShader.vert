#version 330 core

layout (location = 0) in vec3 a_position;

out vec3 v_tex_coord;

uniform mat4 u_view_projection;

void main()
{
    v_tex_coord = a_position;
    gl_Position = u_view_projection * vec4(a_position, 1.0);
    gl_Position = gl_Position.xyww;
}