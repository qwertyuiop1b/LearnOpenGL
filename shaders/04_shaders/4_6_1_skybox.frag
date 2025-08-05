#version 330 core
out vec4 FragColor;

in vec3 vTexCoords;

uniform samplerCube skyboxTexture;


void main() {
    FragColor = texture(skyboxTexture, vTexCoords);
}