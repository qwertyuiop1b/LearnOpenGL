#version 330 core

uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 lightPos;

in vec3 fragPos;
in vec3 vNormal;

out vec4 fragColor;

void main() {
    // 计算向量
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(lightPos - fragPos);
    // 计算夹角
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    // 环境光
    float ambientStrength = 0.15;
    vec3 ambient = ambientStrength * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;

    fragColor = vec4(result, 1.0);
}