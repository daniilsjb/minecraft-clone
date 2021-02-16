#version 430 core

in VertexData {
    vec2 texCoords;
} i;

uniform sampler2D u_atlas;

out vec4 FragColor;

void main() {
    FragColor = texture(u_atlas, i.texCoords);
}