#version 330 core

uniform vec4 uVertexColor;
out vec4 fragColor;

void main() {
    fragColor = uVertexColor;
}