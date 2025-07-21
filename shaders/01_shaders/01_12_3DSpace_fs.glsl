#version 330 core

in vec2 vTexCoord;
uniform sampler2D img1;
uniform sampler2D img2;

out vec4 fragColor;

void main() {
    fragColor = mix(texture(img1, vTexCoord), texture(img2, vTexCoord), 0.2);
}