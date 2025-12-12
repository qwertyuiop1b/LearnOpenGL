#version 330 core

out vec4 fragColor;

in float vIndex;
uniform float uNumber = 12;

void main() {
    fragColor = vec4(vIndex / uNumber, 0.0, 0.0, 1.0);
}