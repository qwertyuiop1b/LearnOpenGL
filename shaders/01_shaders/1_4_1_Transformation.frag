#version 330 core

in vec3 vVertexColor;
in vec2 vTexCoord;

out vec4 fragColor;

uniform sampler2D uTexture;
uniform sampler2D uTexture2;

void main() {
    fragColor = mix(texture(uTexture, vTexCoord), texture(uTexture2, vTexCoord), 0.3);
}