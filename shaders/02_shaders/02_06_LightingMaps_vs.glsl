#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 fragPos;
out vec3 vNormal;
out vec2 texCoords;

void main() {
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0);
    fragPos = vec3(modelMatrix * vec4(aPos, 1.0));
    vNormal =  mat3(transpose(inverse(modelMatrix))) * aNormal;
    texCoords = aTexCoords;
}