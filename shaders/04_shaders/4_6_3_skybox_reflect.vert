#version 330 core
layout (location = 0) in vec3 aPos;
out vec3 vNormal;
out vec3 vPosition;

uniform mat4 projection;
uniform mat4 view;

void main() {
    vec4 position = projection * view * vec4(aPos, 1.0);
    gl_Position = position.xyww;
}