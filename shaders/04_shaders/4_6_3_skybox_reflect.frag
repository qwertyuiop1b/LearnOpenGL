#version 330 core
out vec4 FragColor;

in vec3 vNormal;
in vec3 vPosition;

uniform vec3 cameraPositon;
uniform samplerCube skyboxTexture;


void main() {
    vec3 I = normalize(vPosition - cameraPositon);
    vec3 R = reflect(I, normalize(vNormal));
    FragColor = vec4(texture(skyboxTexture, R).rgb, 1.0);
}