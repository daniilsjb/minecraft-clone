#version 430 core

layout(location = 0) in vec2 a_pos;

uniform vec3 u_color;

out VertexData {
    vec3 color;
} o;

void main() {
	gl_Position = vec4(a_pos, 0.0, 1.0);
    o.color = u_color;
}