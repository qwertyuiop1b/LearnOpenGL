#version 330 core
#define PI 3.14159265358979323846
in vec2 vTexcoord;
out vec4 fragColor;

uniform float uProgress; // 进度值 [0.0, 1.0]

// SDF 圆环函数：返回到圆环的距离（负值表示在圆环内）
float sdfRing(vec2 p, float innerRadius, float outerRadius) {
    float dist = length(p);
    // 如果在内圆内，返回到内圆的距离（正值）
    if (dist < innerRadius) return innerRadius - dist;
    // 如果在外圆外，返回到外圆的距离（正值）
    if (dist > outerRadius) return dist - outerRadius;
    // 如果在圆环内，返回负值（表示在内部）
    return -(min(dist - innerRadius, outerRadius - dist));
}

void main() {
    // 使用 SDF 绘制圆环
    float innerRadius = 0.6;
    float outerRadius = 0.7;
    float ringDist = sdfRing(vTexcoord, innerRadius, outerRadius);
    
    // 如果不在圆环内，完全不显示
    if (ringDist > 0.0) {
        fragColor = vec4(0.0, 0.0, 0.0, 0.0);
        return;
    }
    
    // 计算当前像素的角度（从顶部开始，顺时针）
    // atan 返回 [-pi, pi]，从右侧（x轴正方向）开始
    float angle = atan(vTexcoord.y, vTexcoord.x); // [-pi, pi]
    // 转换为 [0, 2*PI]，从右侧开始
    angle = angle < 0.0 ? angle + 2.0 * PI : angle;
    // 从顶部开始（-90度，即 -PI/2），顺时针
    // 顶部在标准坐标系中是 (0, 1)，atan(1, 0) = PI/2
    // 所以需要减去 PI/2，然后如果为负则加 2*PI
    angle = angle - PI / 2.0;
    if (angle < 0.0) angle += 2.0 * PI;
    // 归一化到 [0, 1]
    float normalizedAngle = angle / (2.0 * PI);
    
    // 判断是否在进度范围内
    bool inProgress = normalizedAngle <= uProgress;
    
    // 圆环边缘平滑（负值表示在圆环内）
    float ringWidth = 0.04;
    float ringAlpha = 1.0 - smoothstep(0.0, ringWidth, abs(ringDist));
    
    // 根据进度和位置决定颜色
    vec3 progressColor = vec3(0.2, 0.8, 1.0); // 进度条颜色（青色）
    vec3 bgColor = vec3(0.3, 0.3, 0.3); // 背景颜色（灰色）
    
    vec3 color = inProgress ? progressColor : bgColor;
    
    fragColor = vec4(color, ringAlpha);
}