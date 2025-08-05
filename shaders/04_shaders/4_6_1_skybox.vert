#version 330 core
layout (location = 0) in vec3 aPos;
out vec3 vTexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    vTexCoords = aPos;
    // Transform the vertex position to clip space
    gl_Position = projection * view * vec4(aPos, 1.0);

}