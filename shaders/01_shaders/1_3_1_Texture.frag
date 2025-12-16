#version 330 core

in vec3 vColor;
in vec2 vTexCoord;

out vec4 fragColor;

uniform sampler2D uTexture;

void main() {
    fragColor = texture(uTexture, vTexCoord) * vec4(vColor, 1.0);
}