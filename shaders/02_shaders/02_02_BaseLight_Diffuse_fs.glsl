#version 330 core

in vec3 vNormal;
in vec3 fragPos;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

out vec4 fragColor;


void main() {
    // 环境光
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    // 漫反射
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    // 相加
    vec3 result = (ambient + diffuse) * objectColor;
    fragColor = vec4(result, 1.0);
}