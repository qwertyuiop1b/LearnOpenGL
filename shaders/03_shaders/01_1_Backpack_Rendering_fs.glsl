#version 330 core
uniform sampler2D texture_diffuse1;
in vec2 vTexcoord;

out vec4 fragColor;

void main() {
    fragColor = texture(texture_diffuse1, vTexcoord);
}