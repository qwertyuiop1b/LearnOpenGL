#version 330 core


in vec3 vNormal;
in vec3 vFragPos;
in vec3 vLightPos;

uniform vec3 lightColor;
uniform vec3 objectColor;

out vec4 FragColor;


void main() {
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(vNormal);
    vec3 lightdir = normalize(vLightPos - vFragPos);
    vec3 diffuse = max(dot(norm, lightdir), 0.0) * lightColor;

    float specularStrength = 0.5;
    vec3 reflectDir = reflect(-lightdir, norm);
    float shininess = 512.0;
    vec3 specular = pow(max(dot(reflectDir, normalize(-vFragPos)), 0.0), shininess) * lightColor;

    FragColor = vec4((ambient + diffuse + specular) * objectColor, 1.0);
}