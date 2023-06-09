#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;


out vec3 outColor;

uniform vec2 offset;
// todo
//uniform mat3 rotateMatrix;


void main() {
    vec3 position = vec3(aPos.xy + offset, 0.);
//    positon = rotateMatrix * position;
    gl_Position = vec4(position, 1.0);
    outColor = aColor;
}