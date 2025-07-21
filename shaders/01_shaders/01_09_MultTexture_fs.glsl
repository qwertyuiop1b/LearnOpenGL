#version 330 core

in vec3 vColor;
in vec2 vTexCoord;

uniform sampler2D img1;
uniform sampler2D img2;
uniform float mixLevel;

out vec4 fragColor;

void main() {
//    fragColor = mix(texture(img1, vTexCoord), texture(img2, vTexCoord), 0.3);
//    fragColor = mix(texture(img1, vTexCoord), texture(img2, vec2(1.0 - vTexCoord.x, vTexCoord.y)), 0.3);
//    fragColor = mix(texture(img1, vTexCoord), texture(img2, vTexCoord * 2), 0.3);

    fragColor = mix(texture(img1, vTexCoord), texture(img2, vTexCoord), mixLevel);
}