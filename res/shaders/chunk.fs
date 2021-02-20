#version 430 core

in VertexData {
    vec2 texCoords;
    vec3 color;
} i;

uniform sampler2D u_atlas;

out vec4 fragColor;

void main() {
    fragColor = texture(u_atlas, i.texCoords) * vec4(i.color, 1.0);
}
