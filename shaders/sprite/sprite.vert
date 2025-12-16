#version 330 core

// 顶点着色器 - Sprite 渲染

layout (location = 0) in vec2 aPos;      // 顶点位置 (x, y)
layout (location = 1) in vec2 aTexCoord; // 纹理坐标 (u, v)

out vec2 TexCoord; // 输出到片段着色器

uniform mat4 uModel;       // 模型矩阵（位置、旋转、缩放）
uniform mat4 uProjection;  // 投影矩阵（可选，默认使用正交投影）

void main()
{
    // 应用模型变换
    gl_Position = uProjection * uModel * vec4(aPos, 0.0, 1.0);
    
    // 传递纹理坐标
    TexCoord = aTexCoord;
}

