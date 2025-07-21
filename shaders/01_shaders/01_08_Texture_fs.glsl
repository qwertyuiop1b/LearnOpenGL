#version 330 core
in vec3 vColor;
in vec2 vTexCoord;
uniform sampler2D imgTexture;

out vec4 fragColor;

void main() {
//    fragColor = texture(imgTexture, vTexCoord);
    fragColor = texture(imgTexture, vTexCoord) * vec4(vColor, 1.0);
}