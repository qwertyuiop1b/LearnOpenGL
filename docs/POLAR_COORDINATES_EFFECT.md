# 极坐标纹理效果实现

基于 [Vertex Fragment 博客文章](https://www.vertexfragment.com/ramblings/polar-coordinates/) 实现的 OpenGL GLSL 版本。

## 效果概述

实现了一个从中心向外扩展的魔法环效果，结合了：
- 极坐标纹理采样
- 程序化噪声生成
- SDF 圆环遮罩
- 时间驱动的溶解效果
- HDR 色调和 Bloom 效果

## 核心技术

### 1. 笛卡尔坐标 → 极坐标转换

```glsl
vec2 CartesianToPolar(vec2 cartesian, vec2 origin) {
    vec2 atOrigin = cartesian - origin;
    float dist = length(atOrigin);
    float angle = atan(atOrigin.y, atOrigin.x);  // [-π, π]
    return vec2(dist, angle);
}
```

### 2. 极坐标采样

```glsl
// 转换坐标
vec2 polar = CartesianToPolar(uv, vec2(0.5));

// 将角度转换为 [0, 1] 范围并添加旋转
polar.y = (polar.y + PI) * ONE_OVER_TWO_PI;
polar.x -= time * movementSpeed;  // 向外扩展
polar.y += time * rotationSpeed;  // 旋转

// 使用 yx 顺序采样（y 是角度，x 是距离）
float noise = noise(polar.yx * frequency);
```

### 3. SDF 圆环遮罩

```glsl
// 根据时间插值半径和厚度
float fade01 = EaseOutQuadratic(fract(time / duration));
float radius = mix(minRadius, maxRadius, fade01);
float thickness = mix(maxThickness, minThickness, fade01);

// SDF 圆环计算
float distance = distance(uv, vec2(0.5));
float ring = 1.0 - pow(clamp(abs(distance - radius) / thickness, 0.0, 1.0), softness);
```

### 4. 时间驱动的溶解

```glsl
// 结合噪声和环
float combined = noise * ring;

// 随时间推移，只有高值保持可见（溶解效果）
float final = clamp((combined - fade01) * strength, 0.0, 1.0);
```

### 5. 缓动函数

```glsl
// EaseOutQuadratic: https://easings.net/
float EaseOutQuadratic(float x) {
    return 1.0 - pow(1.0 - x, 2.0);
}
```

## 参数调整指南

### 时间参数
- `fadeDuration` (3.0s): 扩展周期，值越大环扩展越慢
- `movementSpeed` (0.3): 向外移动速度，值越大扩展越快
- `rotationSpeed` (0.1): 旋转速度，产生漩涡效果

### 视觉效果
- `smoothness` (2.0): 噪声对比度，值越大对比越强
- `ringSoftness` (8.0): 环边缘锐利度，值越大边缘越锐利
- `finalStrength` (3.0): 整体强度，控制亮度和对比度

### 环尺寸
```cpp
// {MinRadius, MaxRadius, MinThickness, MaxThickness}
{0.0f, 0.5f, 0.2f, 0.05f}
```
- `MinRadius`: 起始半径（中心）
- `MaxRadius`: 最大半径（边缘）
- `MinThickness`: 起始厚度（较厚）
- `MaxThickness`: 结束厚度（变薄消失）

### 颜色预设

**魔法效果（默认）：**
```cpp
{0.3f, 1.5f, 2.0f, 1.0f}  // 青色 HDR，强 Bloom
```

**水环效果：**
```cpp
{0.5f, 0.8f, 1.0f, 0.3f}  // 柔和半透明
fadeDuration = 1.5f;       // 更快
ringSoftness = 4.0f;       // 更柔和
```

**火焰效果：**
```cpp
{2.0f, 1.0f, 0.3f, 1.0f}  // 橙红色
rotationSpeed = 0.3f;      // 更快旋转
```

**能量波效果：**
```cpp
{1.0f, 2.0f, 1.0f, 1.0f}  // 绿色
finalStrength = 5.0f;      // 更强对比
```

## 程序化噪声

由于没有纹理，使用简化的 hash 函数生成噪声：

```glsl
float hash(vec2 p) {
    p = fract(p * vec2(443.537, 537.247));
    p += dot(p, p + 19.19);
    return fract(p.x * p.y);
}

float noise(vec2 p) {
    vec2 i = floor(p);
    vec2 f = fract(p);
    f = f * f * (3.0 - 2.0 * f); // smoothstep
    
    // 双线性插值
    float a = hash(i);
    float b = hash(i + vec2(1.0, 0.0));
    float c = hash(i + vec2(0.0, 1.0));
    float d = hash(i + vec2(1.0, 1.0));
    
    return mix(mix(a, b, f.x), mix(c, d, f.x), f.y);
}
```

## 进阶：使用噪声纹理

如果有噪声纹理，可以替换程序化噪声：

```glsl
uniform sampler2D uNoiseTexture;

// 在 main() 中
float sampled = texture(uNoiseTexture, polar.yx).r;
```

## 关键差异：HLSL vs GLSL

| HLSL | GLSL |
|------|------|
| `ddx` / `ddy` | `dFdx` / `dFdy` |
| `saturate()` | `clamp(x, 0.0, 1.0)` |
| `lerp()` | `mix()` |
| `frac()` | `fract()` |
| `atan2(y, x)` | `atan(y, x)` |

## 性能优化

1. **程序化噪声**：实时计算，无需纹理内存
2. **早期剔除**：通过 alpha 测试提前返回
3. **平滑插值**：使用 smoothstep 而非线性插值
4. **GPU 导数**：使用 `fwidth` 实现自适应抗锯齿（如需要）

## 应用场景

- 魔法技能特效
- 水面涟漪
- 冲击波效果
- 传送门动画
- 能量护盾
- 爆炸冲击波

## 扩展建议

1. **多环效果**：同时渲染多个环，使用不同的时间偏移
2. **纹理变体**：加载实际的噪声纹理或 Perlin 噪声
3. **法线扰动**：使用噪声影响环的法线，产生波纹感
4. **粒子系统**：结合粒子产生更丰富的效果
5. **交互**：根据玩家位置动态生成环

## 参考资源

- 原始博客：https://www.vertexfragment.com/ramblings/polar-coordinates/
- 缓动函数：https://easings.net/
- SDF 技术：https://iquilezles.org/articles/distfunctions2d/

## 编译运行

```bash
cd /home/hhq/code/personal/LearnOpenGL
cmake --build build --target 1_2_4_GlslPolar
./build/1_2_4_GlslPolar
```

