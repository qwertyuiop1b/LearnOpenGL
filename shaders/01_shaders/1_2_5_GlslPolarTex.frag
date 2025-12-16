#version 330 core

// ============================================================================
// 极坐标纹理效果 Fragment Shader
// 参考：https://www.vertexfragment.com/ramblings/polar-coordinates/
// ============================================================================

// 常量定义
#define PI 3.14159265358979323846
#define ONE_OVER_TWO_PI 0.159155  // 1 / (2*PI)，用于快速计算

// 输入输出
in vec2 vTexcoord;     // 从顶点着色器传来的 UV 坐标 [0,1]
out vec4 fragColor;    // 输出到屏幕的颜色

// Uniform 变量（从 CPU 传递过来的参数，所有片段共享）
uniform float uTime;          // 当前时间（秒）
uniform vec4 uRingDimensions; // 环的尺寸 (最小半径, 最大半径, 最大厚度, 最小厚度)
uniform float uFadeDuration;  // 一个完整循环的持续时间（秒）
uniform float uMovementSpeed; // 噪声向外扩展的速度
uniform float uRotationSpeed; // 噪声旋转的速度
uniform float uSmoothness;    // 噪声的平滑度/对比度
uniform float uRingSoftness;  // 环边缘的柔和度
uniform float uFinalStrength; // 最终效果的强度
uniform vec4 uTint;           // 色调（可以大于 1.0 产生 HDR 效果）

// ============================================================================
// 工具函数部分
// ============================================================================

/**
 * 缓动函数：EaseOutQuadratic
 * 
 * 什么是缓动？
 * - 想象一个球从高处落下，开始很快，然后慢慢减速停止
 * - 缓动函数让动画看起来更自然，而不是机械的匀速
 * 
 * 这个函数做什么？
 * - 输入：0 到 1 之间的值（比如时间进度）
 * - 输出：经过"缓动"处理的值，开始快，结束慢
 * - 图示：https://easings.net/#easeOutQuad
 * 
 * 数学原理：
 * - 原始：y = x（直线）
 * - 缓动后：y = 1 - (1-x)²（曲线）
 */
float EaseOutQuadratic(float x) {
    return 1.0 - pow(1.0 - x, 2.0);
}

/**
 * 笛卡尔坐标 → 极坐标转换
 * 
 * 什么是笛卡尔坐标？
 * - 我们平时用的 (x, y) 坐标系
 * - 比如屏幕上的像素位置
 * 
 * 什么是极坐标？
 * - 用 (距离, 角度) 来表示位置
 * - 距离：离中心点多远
 * - 角度：相对于中心点的方向（-π 到 π）
 * 
 * 为什么要转换？
 * - 极坐标更适合做圆形、旋转、扩散等效果
 * - 就像用圆规画圆比用直尺容易一样
 * 
 * 参数：
 * - cartesian: 笛卡尔坐标 (x, y)
 * - origin: 极坐标的原点（中心）
 * 
 * 返回：
 * - vec2(距离, 角度)
 */
vec2 CartesianToPolar(vec2 cartesian, vec2 origin) {
    // 1. 将坐标移动到以 origin 为中心
    vec2 atOrigin = cartesian - origin;
    
    // 2. 计算距离（勾股定理：√(x² + y²)）
    float dist = length(atOrigin);
    
    // 3. 计算角度（反正切函数）
    // atan(y, x) 返回 [-π, π]
    // - 右边 (1, 0) = 0
    // - 上面 (0, 1) = π/2
    // - 左边 (-1, 0) = π
    // - 下面 (0, -1) = -π/2
    float angle = atan(atOrigin.y, atOrigin.x);
    
    return vec2(dist, angle);
}

/**
 * 程序化噪声：Hash 函数
 * 
 * 什么是 Hash？
 * - 把输入"打乱"成看起来随机的输出
 * - 相同输入永远得到相同输出（伪随机）
 * 
 * 用途：
 * - 作为噪声函数的基础
 * - 生成看起来随机但可重复的图案
 */
float hash(vec2 p) {
    // 使用魔法数字（质数）打乱坐标
    p = fract(p * vec2(443.537, 537.247));
    p += dot(p, p + 19.19);
    return fract(p.x * p.y);
}

/**
 * 程序化噪声函数
 * 
 * 什么是噪声（Noise）？
 * - 不是纯随机，而是"平滑的随机"
 * - 像云朵、大理石纹理那样的自然图案
 * 
 * 这个函数做什么？
 * - 生成值噪声（Value Noise）
 * - 对四个角的 hash 值进行双线性插值
 * 
 * 过程：
 * 1. 找到当前点所在的网格
 * 2. 计算网格四个角的随机值
 * 3. 根据位置在四个值之间平滑插值
 * 
 * 为什么要这样？
 * - 直接用 hash 会得到像素块
 * - 插值让过渡变得平滑，像真实纹理
 */
float noise(vec2 p) {
    // 1. 分离整数和小数部分
    vec2 i = floor(p);    // 整数部分：确定在哪个网格
    vec2 f = fract(p);    // 小数部分：在网格内的位置 [0,1]
    
    // 2. 平滑曲线（Smoothstep）
    // 为什么？让插值更自然，避免方向性
    // f = f * f * (3 - 2*f) 是 smoothstep 的公式
    f = f * f * (3.0 - 2.0 * f);
    
    // 3. 获取网格四个角的随机值
    //    c --- d
    //    |  *  |  (* 是当前点)
    //    a --- b
    float a = hash(i);                    // 左下角
    float b = hash(i + vec2(1.0, 0.0));  // 右下角
    float c = hash(i + vec2(0.0, 1.0));  // 左上角
    float d = hash(i + vec2(1.0, 1.0));  // 右上角
    
    // 4. 双线性插值
    // 先在 x 方向插值两次，再在 y 方向插值一次
    return mix(
        mix(a, b, f.x),  // 底边插值
        mix(c, d, f.x),  // 顶边插值
        f.y              // 在两条边之间插值
    );
}

// ============================================================================
// 主函数
// ============================================================================
void main() {
    
    // ========================================================================
    // 第一步：极坐标采样噪声
    // ========================================================================
    
    // 1.1 将 UV 坐标转换为极坐标
    // UV 范围 [0,1]，我们要以 (0.5, 0.5) 为中心
    float uvScale = 2.0;  // 缩放 UV 来控制效果大小
    vec2 scaledUV = vTexcoord * uvScale;
    vec2 polar = CartesianToPolar(scaledUV, vec2(0.5) * uvScale);
    
    // polar.x = 距离（从中心向外 0 到 ~0.7）
    // polar.y = 角度（-π 到 π）
    
    // 1.2 转换角度范围
    // 从 [-π, π] 转到 [0, 1]，这样更容易使用
    // 为什么加 π 再除以 2π？
    // - 原来：-π 到 π（范围 2π）
    // - 加 π：0 到 2π
    // - 除 2π：0 到 1
    polar.y = (polar.y + PI) * ONE_OVER_TWO_PI;
    
    // 1.3 添加动画：移动和旋转
    // 为什么要动？让效果看起来活跃，不呆板
    polar.x -= uTime * uMovementSpeed;  // 距离减小 = 向外扩展
    polar.y += uTime * uRotationSpeed;  // 角度增加 = 顺时针旋转
    
    // 1.4 采样噪声纹理
    // 为什么用 polar.yx 而不是 polar.xy？
    // - polar.y 是角度（0到1）→ 横向 u
    // - polar.x 是距离 → 纵向 v
    // - 这样噪声会沿着圆周方向排列
    float sampled = noise(polar.yx * 8.0);  // 8.0 控制噪声密度
    
    // 1.5 调整噪声对比度
    // pow(x, n)：n > 1 会让暗的更暗，亮的保持亮
    // 效果：增加对比度，让图案更清晰
    sampled = pow(sampled, uSmoothness);
    
    // ========================================================================
    // 第二步：SDF 圆环遮罩
    // ========================================================================
    
    // 2.1 计算当前时间在循环中的位置
    // fract(uTime / uFadeDuration) 得到 0 到 1 的循环值
    // 比如：0秒→0, 1.5秒→0.5, 3秒→1, 3.1秒→0.033...
    float fade01 = EaseOutQuadratic(fract(uTime / uFadeDuration));
    
    // 2.2 根据时间插值环的参数
    // 为什么要插值？创建"扩散"效果
    // - 开始：小半径、厚环
    // - 结束：大半径、薄环
    float ringRadius = mix(
        uRingDimensions.x,  // 最小半径
        uRingDimensions.y,  // 最大半径
        fade01              // 插值因子
    );
    float ringThickness = mix(
        uRingDimensions.z,  // 最大厚度（开始时厚）
        uRingDimensions.w,  // 最小厚度（结束时薄）
        fade01
    );
    
    // 2.3 计算像素到中心的距离
    float ringDistance = distance(vTexcoord, vec2(0.5));
    
    // 2.4 计算环遮罩（SDF - Signed Distance Field）
    // SDF 是什么？
    // - 表示"离形状边界的距离"
    // - 负值 = 内部，正值 = 外部，0 = 边界
    // 
    // abs(ringDistance - ringRadius)：离环中心线的距离
    // 除以 ringThickness：归一化到 0-1
    // clamp：限制在 0-1 范围
    // pow：锐化边缘（值越大边缘越锐利）
    // 1.0 -：反转（0变1，1变0）
    float ring = 1.0 - pow(
        clamp(abs(ringDistance - ringRadius) / ringThickness, 0.0, 1.0),
        uRingSoftness
    );
    
    // ========================================================================
    // 第三步：组合噪声和环
    // ========================================================================
    
    // 简单相乘：只在环的位置显示噪声
    // ring = 0 的地方 → 完全透明
    // ring = 1 的地方 → 完全显示噪声
    float combined = sampled * ring;
    
    // ========================================================================
    // 第四步：时间驱动的溶解
    // ========================================================================
    
    // 这是关键效果！
    // 随着 fade01 从 0 增长到 1：
    // - combined - fade01 的值越来越小
    // - 只有噪声中最亮的部分能"存活"
    // - 创造出破碎、消散的效果
    // 
    // uFinalStrength：控制对比度
    // - 值越大，边界越锐利
    // - 值越小，过渡越柔和
    float final = clamp((combined - fade01) * uFinalStrength, 0.0, 1.0);
    
    // ========================================================================
    // 第五步：应用颜色和输出
    // ========================================================================
    
    // 将灰度值转换为彩色
    // uTint 可以大于 1.0（HDR），配合 Bloom 后期处理产生发光效果
    fragColor = vec4(final, final, final, final) * uTint;
}

