#version 330 core

in vec2 vTexcoords;
uniform sampler2D texture1;

out vec4 fragColor;

void main() {
    // fragColor = texture(texture1, vTexcoords);
    fragColor = vec4(vec3(gl_FragCoord.z), 1.0);
}