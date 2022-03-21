#version 430 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coord;

uniform mat4 u_view_projection;
uniform mat4 u_transform;
uniform mat3 u_normal;

out vec2 v_tex_coord;
out vec3 v_normal;
out vec3 v_position;
out vec4 v_pos;

void main()
{
	v_pos = u_view_projection * u_transform * vec4(a_position, 1.0);
	gl_Position = v_pos;
	v_tex_coord = a_tex_coord;
	v_normal = u_normal * a_normal;
	v_position = vec3(u_transform * vec4(a_position, 1.0));
}
