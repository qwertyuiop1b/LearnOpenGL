#version 330 core

// 顶点着色器：负责处理顶点位置
// 这里只是简单传递位置和 UV 坐标，不做复杂处理

layout(location = 0) in vec3 aPos;  // 顶点位置输入

out vec2 vTexcoord;  // 传递给片段着色器的 UV 坐标

void main() {
    // 直接设置顶点位置（已经是 NDC 坐标 -1 到 1）
    gl_Position = vec4(aPos, 1.0);
    
    // 将顶点位置转换为 UV 坐标（从 [-1,1] 转到 [0,1]）
    vTexcoord = aPos.xy * 0.5 + 0.5;
}

