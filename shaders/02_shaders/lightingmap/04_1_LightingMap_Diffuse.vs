#version 300 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexcoord;
layout(location = 2) in vec3 aNormal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 vTexcoord;
out vec3 vNormal;
out vec3 vFragPos;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vFragPos = vec3(model * vec4(aPos, 1.0));
    vTexcoord = aTexcoord;
    vNormal = mat3(transpose(inverse(modelMatrix))) * aNormal
}