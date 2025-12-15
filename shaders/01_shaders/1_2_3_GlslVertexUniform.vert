#version 330 core
layout(location = 0) in vec3 aPos;

uniform vec2 uOffset;

void main() {

    gl_Position = vec4(uOffset + aPos.xy, aPos.z, 1.0);
}