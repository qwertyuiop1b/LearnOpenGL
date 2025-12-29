#version 330 core

in vec3 vFragPos;
in vec3 vNormal;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 objectColor;

out vec4 FragColor;


void main() {
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 direction = normalize(lightPos - vFragPos);
    vec3 normal = normalize(vNormal);
    float diff = max(dot(normal, direction), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
}