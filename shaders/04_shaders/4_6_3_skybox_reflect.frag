#version 330 core
out vec4 FragColor;

in vec3 vPosition;
in vec3 vNormal;

uniform vec3 cameraPosition;
uniform samplerCube skyboxTexture;


void main() {
    vec3 I = normalize(vPosition - cameraPosition);
    vec3 R = reflect(I, normalize(vNormal));
    FragColor = vec4(texture(skyboxTexture, R).rgb, 1.0);
}