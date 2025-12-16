#version 330 core

// 片段着色器 - Sprite 渲染

in vec2 TexCoord; // 从顶点着色器传来的纹理坐标

out vec4 FragColor; // 输出颜色

uniform sampler2D uTexture; // 精灵图纹理
uniform vec3 uColor;        // 颜色调制（默认白色）

void main()
{
    // 采样纹理
    vec4 texColor = texture(uTexture, TexCoord);
    
    // 应用颜色调制
    FragColor = vec4(uColor, 1.0) * texColor;
    
    // 如果完全透明，丢弃片段（可选）
    if (FragColor.a < 0.01)
        discard;
}

