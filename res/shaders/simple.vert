#version 430 core

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec2 in_texCoords;

out vec2 TexCoords;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

void main()
{
	TexCoords = in_texCoords;
	gl_Position = u_projection * u_view * u_model * vec4(in_pos, 1.0);
}
