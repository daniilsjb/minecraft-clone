#version 430 core

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec2 a_texCoords;
layout(location = 2) in vec3 a_color;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

out VertexData {
    vec2 texCoords;
    vec3 color;
} o;

void main() {
    gl_Position = u_projection * u_view * u_model * vec4(a_pos, 1.0);
    o.texCoords = a_texCoords;
    o.color = a_color;
}
