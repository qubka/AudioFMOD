#version 330 core

layout (location = 0) out vec4 o_color;

in vec2 v_tex_coord;

uniform sampler2D atlas;
uniform vec4 color;

void main()
{
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture2D(atlas, v_tex_coord).r);
	o_color = color * sampled;
}