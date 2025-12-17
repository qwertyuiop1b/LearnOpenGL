# 火焰精灵动画示例

## 概览

这个示例展示了如何使用 Sprite 动画系统播放火焰动画效果。

## 精灵图信息

**文件**: `textures/fire_frame.jpg`

**规格**:
- 尺寸：1920 x 1920 像素
- 布局：6 x 6 网格
- 总帧数：36 帧
- 每帧大小：320 x 320 像素
- 格式：JPEG

**布局示意**:
```
┌────────────────────────────────────┐
│ [0]  [1]  [2]  [3]  [4]  [5]      │  Row 0
│ [6]  [7]  [8]  [9]  [10] [11]     │  Row 1
│ [12] [13] [14] [15] [16] [17]     │  Row 2
│ [18] [19] [20] [21] [22] [23]     │  Row 3
│ [24] [25] [26] [27] [28] [29]     │  Row 4
│ [30] [31] [32] [33] [34] [35]     │  Row 5
└────────────────────────────────────┘
```

## 动画配置

### 动画 1: Fire（完整火焰）
- **帧序列**: 0-35（所有 36 帧）
- **帧率**: 15 FPS
- **循环**: 是
- **效果**: 平滑、完整的火焰燃烧动画

### 动画 2: Fire Fast（快速火焰）
- **帧序列**: 0, 2, 4, 6, ... 34（偶数帧，共 18 帧）
- **帧率**: 20 FPS
- **循环**: 是
- **效果**: 更快、更激烈的火焰效果

## 代码关键部分

### 帧定义
```cpp
// 自动添加 6x6 网格的所有 36 帧
spriteSheet->addFrameGrid(
    0, 0,       // 起始位置（左上角）
    320, 320,   // 每帧大小
    6, 6        // 6 列 6 行
);
```

### 动画创建
```cpp
// 完整火焰动画（所有帧）
std::vector<int> fireFrames;
for (int i = 0; i < 36; ++i) {
    fireFrames.push_back(i);
}
Sprite::Animation fireAnim("fire", fireFrames, 15.0f);

// 快速火焰动画（跳帧）
std::vector<int> fastFireFrames;
for (int i = 0; i < 36; i += 2) {
    fastFireFrames.push_back(i);
}
Sprite::Animation fastFireAnim("fire_fast", fastFireFrames, 20.0f);
```

## 运行示例

```bash
cd build
./1_3_5_SpriteAnimation
```

## 控制说明

### ImGui 控制面板

**动画切换**:
- `Fire (36 frames)` - 完整的火焰动画
- `Fire Fast (18 frames)` - 快速版本

**速度控制**:
- 滑块范围：0.1x - 5.0x
- 1.0x = 正常速度
- 可以实现慢动作或快速播放

**播放控制**:
- `Play` / `Pause` - 播放/暂停
- `Reset` - 重置到第一帧
- `Loop` - 循环开关

### 状态显示
- 当前动画名称
- 当前帧 / 总帧数
- FPS（帧率）
- 播放状态（暂停/完成/循环）

## 渲染设置

**精灵渲染**:
```cpp
glm::vec2 position(0.0f, 0.0f);  // 屏幕中心
glm::vec2 size(2.0f, 2.0f);      // 2x2 单位大小
renderer->render(position, size, 0.0f, glm::vec3(1.0f));
```

**投影设置**:
- 正交投影：`ortho(-2.0, 2.0, -2.0, 2.0, -1.0, 1.0)`
- 坐标系：中心为原点

**混合设置**:
```cpp
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
```

## 性能信息

- 目标帧率：60 FPS
- 渲染方式：单个 quad（6 个顶点）
- 纹理切换：每帧更新 UV 坐标
- GPU 负载：极低（单个精灵）

## 自定义建议

### 调整动画速度
```cpp
// 更慢的火焰（10 FPS）
Sprite::Animation slowFire("fire_slow", fireFrames, 10.0f);

// 非常快的火焰（30 FPS）
Sprite::Animation veryFastFire("fire_very_fast", fireFrames, 30.0f);
```

### 创建不同的帧序列
```cpp
// 只使用前半部分帧
std::vector<int> halfFrames = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};

// 倒放动画
std::vector<int> reverseFrames;
for (int i = 35; i >= 0; --i) {
    reverseFrames.push_back(i);
}

// 来回播放
std::vector<int> pingPongFrames;
for (int i = 0; i < 36; ++i) pingPongFrames.push_back(i);
for (int i = 34; i >= 1; --i) pingPongFrames.push_back(i);
```

### 调整精灵大小和位置
```cpp
// 更大的火焰
renderer->render(glm::vec2(0.0f, 0.0f), glm::vec2(3.0f, 3.0f));

// 偏移位置
renderer->render(glm::vec2(1.0f, -0.5f), glm::vec2(2.0f, 2.0f));

// 旋转
float rotation = glm::radians(45.0f);
renderer->render(position, size, rotation);

// 颜色调制（蓝色火焰）
renderer->render(position, size, 0.0f, glm::vec3(0.5f, 0.5f, 1.0f));
```

## 故障排除

### 问题：火焰显示不正确

**可能原因**:
1. 帧大小设置错误（应该是 320x320，不是 64x64）
2. 网格参数不匹配

**解决方法**:
```cpp
// 确认精灵图尺寸
std::cout << "Texture: " << spriteSheet->getTextureWidth() 
          << "x" << spriteSheet->getTextureHeight() << std::endl;

// 确认帧数量
std::cout << "Total frames: " << spriteSheet->getFrameCount() << std::endl;
```

### 问题：动画太快或太慢

**解决方法**:
1. 调整 FPS 参数：`Animation("fire", frames, 15.0f)` ← 修改这个数字
2. 使用速度倍率：`animation->setSpeed(2.0f)` ← 运行时调整
3. 使用 ImGui 滑块实时调整

### 问题：动画卡顿

**可能原因**:
- deltaTime 不准确
- 帧率限制

**解决方法**:
```cpp
// 检查 deltaTime
std::cout << "DeltaTime: " << deltaTime << std::endl;

// 确认 FPS 限制
fps->setTargetFrameRate(60);  // 或更高
```

## 扩展想法

1. **多个火焰实例**
   - 创建多个渲染器
   - 在不同位置渲染多个火焰

2. **粒子效果**
   - 结合粒子系统
   - 添加火花和烟雾

3. **交互**
   - 鼠标悬停改变颜色
   - 点击暂停/播放
   - 键盘控制动画切换

4. **着色器效果**
   - 自定义片段着色器
   - 添加发光效果
   - 热扭曲效果

## 相关文件

- `src/01_Start/1_3_5_SpriteAnimation.cpp` - 主应用代码
- `src/sprite/` - Sprite 动画系统源码
- `shaders/sprite/` - Sprite 着色器
- `SPRITE_ANIMATION_GUIDE.md` - 完整的系统文档
- `textures/SPRITE_SHEET_HOWTO.md` - 精灵图准备指南

## 版权信息

**火焰精灵图**:
- 文件：fire_frame.jpg
- 创建：Adobe Photoshop CS6
- 日期：2014年12月6日

## 技术栈

- OpenGL 3.3+
- C++17
- GLFW (窗口管理)
- GLAD (OpenGL 加载器)
- GLM (数学库)
- ImGui (UI 界面)
- stb_image (图像加载)

---

**享受你的火焰动画！** 🔥✨

