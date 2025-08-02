#version 330 core
uniform sampler2D quadTexture;
in vec2 vTexcoords;

out vec4 FragColor;
void main() {
    // 渲染完全透明图像部分会显示白色
    // FragColor = texture(quadTexture, vTexcoords);

    // 边缘有些白边，需要设置采样贴图，因为默认是repeat，改成clamp to edge
    vec4 color = texture(quadTexture, vTexcoords);
    if (color.a < 0.1) {
        discard; // 丢弃透明部分
    }
    FragColor = color;
}