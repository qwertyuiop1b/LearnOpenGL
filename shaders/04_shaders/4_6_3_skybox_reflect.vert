#version 330 core
layout (location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;

out vec3 vPosition;
out vec3 vNormal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
    vNormal = mat3(transpose(inverse(model))) * aNormal;
    vPosition = vec3(model * vec4(aPosition, 1.0));
    gl_Position = projection * view * vec4(vPosition, 1.0);
}