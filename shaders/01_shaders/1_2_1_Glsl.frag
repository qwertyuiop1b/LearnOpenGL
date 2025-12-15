#version 330 core

in vec4 vVertexColor;
out vec4 fragColor;

void main() {
    fragColor = vVertexColor;
}