#version 430 core

in VertexData {
    vec3 color;
} i;

out vec4 FragColor;

void main() {
    FragColor = vec4(i.color, 1.0);
}