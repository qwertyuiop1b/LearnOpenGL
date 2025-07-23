#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 vNormal;
out vec3 fragPos;

void main() {
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0);
    fragPos = vec3(modelMatrix * vec4(aPos, 1.0));
    // 法线矩阵：模型矩阵的逆矩阵的转置矩阵
    vNormal = mat3(transpose(inverse(modelMatrix))) * aNormal;  // 放在cpu构建
}