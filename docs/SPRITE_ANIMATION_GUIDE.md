# Sprite 动画系统使用指南

## 目录

1. [系统概述](#系统概述)
2. [核心类介绍](#核心类介绍)
3. [快速开始](#快速开始)
4. [详细教程](#详细教程)
5. [不规则布局最佳实践](#不规则布局最佳实践)
6. [API 参考](#api-参考)
7. [常见问题](#常见问题)

---

## 系统概述

Sprite 动画系统是一个完整的 2D 精灵图（Sprite Sheet）动画解决方案，支持：

- ✅ **不规则布局**：手动定义每帧的位置和大小
- ✅ **规则网格**：快速定义网格布局的帧
- ✅ **多动画管理**：一个精灵图支持多个动画序列
- ✅ **播放控制**：暂停、播放、重置、速度调节
- ✅ **循环模式**：支持循环和单次播放
- ✅ **实时更新**：基于 deltaTime 的平滑动画

### 系统架构

```
┌─────────────────┐
│ SpriteSheet     │  加载纹理和定义帧
│ - texture       │
│ - frames[]      │
└────────┬────────┘
         │
         │ 被引用
         │
┌────────▼────────┐
│ Animation       │  管理动画序列
│ - frameIndices[]│
│ - speed, loop   │
└────────┬────────┘
         │
         │ 被使用
         │
┌────────▼────────┐
│ SpriteRenderer  │  渲染和更新
│ - animations{}  │
│ - shader, VAO   │
└─────────────────┘
```

---

## 核心类介绍

### 1. SpriteSheet

**职责**：管理精灵图纹理和帧定义

**关键方法**：
- `addFrame(x, y, w, h)` - 添加单个帧
- `addFrameGrid(...)` - 批量添加网格帧
- `getFrame(index)` - 获取指定帧
- `bind(unit)` - 绑定纹理

### 2. Animation

**职责**：定义和管理动画序列

**关键方法**：
- `update(deltaTime)` - 更新动画状态
- `play() / pause()` - 播放控制
- `setSpeed(multiplier)` - 调整速度
- `setLooping(bool)` - 设置循环
- `getCurrentFrame()` - 获取当前帧索引

### 3. SpriteRenderer

**职责**：渲染精灵并管理多个动画

**关键方法**：
- `addAnimation(anim)` - 添加动画
- `setAnimation(name)` - 切换动画
- `update(deltaTime)` - 更新当前动画
- `render(pos, size, rotation)` - 渲染当前帧

---

## 快速开始

### 步骤 1：准备精灵图

创建或获取一张精灵图（Sprite Sheet）：

```
示例布局：
┌───────────────────────────────┐
│ [0] [1] [2] [3]               │ Row 0: Idle 动画
│ [4] [5] [6] [7] [8] [9]       │ Row 1: Walk 动画
│ [10][11][12][13]              │ Row 2: Jump 动画
└───────────────────────────────┘
```

保存为 `textures/character_sheet.png`

### 步骤 2：加载精灵图并定义帧

```cpp
#include "sprite/SpriteSheet.h"

// 创建精灵图
auto spriteSheet = std::make_unique<Sprite::SpriteSheet>("textures/character_sheet.png");

// 方式 1：手动定义帧（不规则布局）
spriteSheet->addFrame(0, 0, 64, 64);     // Frame 0
spriteSheet->addFrame(64, 0, 64, 64);    // Frame 1
spriteSheet->addFrame(128, 0, 64, 64);   // Frame 2
// ...

// 方式 2：网格定义（规则布局）
spriteSheet->addFrameGrid(0, 0, 64, 64, 4, 1);    // Idle: 4 帧，1 行
spriteSheet->addFrameGrid(0, 64, 64, 64, 6, 1);   // Walk: 6 帧，1 行
spriteSheet->addFrameGrid(0, 128, 64, 64, 4, 1);  // Jump: 4 帧，1 行
```

### 步骤 3：创建动画

```cpp
#include "sprite/Animation.h"

// Idle 动画：帧 0-3，10 FPS
Sprite::Animation idleAnim("idle", {0, 1, 2, 3}, 10.0f);
idleAnim.setLooping(true);

// Walk 动画：帧 4-9，12 FPS
Sprite::Animation walkAnim("walk", {4, 5, 6, 7, 8, 9}, 12.0f);
walkAnim.setLooping(true);

// Jump 动画：帧 10-13，15 FPS，不循环
Sprite::Animation jumpAnim("jump", {10, 11, 12, 13}, 15.0f);
jumpAnim.setLooping(false);
```

### 步骤 4：创建渲染器并添加动画

```cpp
#include "sprite/SpriteRenderer.h"

// 创建渲染器
auto renderer = std::make_unique<Sprite::SpriteRenderer>(spriteSheet.get());

// 添加动画
renderer->addAnimation(idleAnim);
renderer->addAnimation(walkAnim);
renderer->addAnimation(jumpAnim);

// 设置初始动画
renderer->setAnimation("idle");
```

### 步骤 5：主循环

```cpp
// 每帧更新和渲染
while (!window->shouldClose()) {
    float deltaTime = fps->getDeltaTime();
    
    // 更新动画
    renderer->update(deltaTime);
    
    // 渲染
    glClear(GL_COLOR_BUFFER_BIT);
    renderer->render(
        glm::vec2(0.0f, 0.0f),  // 位置
        glm::vec2(1.0f, 1.0f),  // 大小
        0.0f                     // 旋转
    );
    
    window->swapBuffer();
}
```

---

## 详细教程

### 定义帧的方法

#### 方法 1：手动定义（适用于不规则布局）

当精灵图中的每帧位置和大小不同时：

```cpp
// 参数：x, y, width, height（像素坐标，左上角为原点）
spriteSheet->addFrame(10, 20, 64, 72);   // 不同大小
spriteSheet->addFrame(80, 15, 70, 80);   // 不同位置
spriteSheet->addFrame(160, 20, 64, 72);  // 不同位置
```

**坐标系统**：
- 原点在左上角
- X 轴向右
- Y 轴向下
- 单位：像素

#### 方法 2：网格定义（适用于规则布局）

当精灵图是规则网格时：

```cpp
// 参数：startX, startY, frameWidth, frameHeight, cols, rows, count
spriteSheet->addFrameGrid(
    0, 0,       // 起始位置
    64, 64,     // 每帧大小
    8, 2,       // 8 列 2 行
    -1          // -1 = 添加所有帧（16 帧）
);

// 只添加部分帧
spriteSheet->addFrameGrid(0, 128, 64, 64, 8, 1, 5);  // 只添加前 5 帧
```

### 创建动画

#### 基础动画

```cpp
// 构造函数：Animation(name, frameIndices, fps)
Sprite::Animation anim("walk", {0, 1, 2, 3, 4, 5}, 12.0f);
```

#### 设置属性

```cpp
anim.setLooping(true);        // 循环播放
anim.setSpeed(2.0f);          // 2 倍速
anim.setFPS(15.0f);           // 15 FPS
```

#### 控制播放

```cpp
anim.play();                  // 播放/继续
anim.pause();                 // 暂停
anim.reset();                 // 重置到第一帧
```

#### 查询状态

```cpp
int currentFrame = anim.getCurrentFrame();    // 当前帧索引
bool isPaused = anim.isPaused();              // 是否暂停
bool isFinished = anim.isFinished();          // 是否完成（非循环）
```

### 渲染精灵

#### 基本渲染

```cpp
renderer->render(
    glm::vec2(0.0f, 0.0f),  // 世界坐标位置
    glm::vec2(1.0f, 1.0f),  // 尺寸
    0.0f,                    // 旋转角度（弧度）
    glm::vec3(1.0f)         // 颜色调制（可选）
);
```

#### 带旋转

```cpp
#include <glm/gtc/constants.hpp>

renderer->render(
    glm::vec2(0.0f, 0.0f),
    glm::vec2(1.0f, 1.0f),
    glm::radians(45.0f),    // 旋转 45 度
    glm::vec3(1.0f)
);
```

#### 颜色调制

```cpp
// 红色调
renderer->render(
    glm::vec2(0.0f, 0.0f),
    glm::vec2(1.0f, 1.0f),
    0.0f,
    glm::vec3(1.0f, 0.5f, 0.5f)  // 红色调制
);
```

---

## 不规则布局最佳实践

### 示例 1：不同大小的帧

```cpp
// 精灵图布局：
// - 大角色：120x120 像素
// - 小角色：60x60 像素
// - 特效：80x80 像素

spriteSheet->addFrame(0, 0, 120, 120);      // 大角色
spriteSheet->addFrame(120, 0, 60, 60);      // 小角色
spriteSheet->addFrame(180, 0, 80, 80);      // 特效
```

### 示例 2：紧密打包的精灵图

```cpp
// 使用工具（如 TexturePacker）生成的布局
// 手动记录每帧的坐标

struct FrameData {
    int x, y, w, h;
    const char* name;
};

FrameData frames[] = {
    {10, 5, 64, 72, "idle_0"},
    {80, 5, 64, 72, "idle_1"},
    {150, 5, 64, 72, "idle_2"},
    {10, 85, 70, 80, "walk_0"},
    {85, 85, 70, 80, "walk_1"},
    // ...
};

for (const auto& frame : frames) {
    spriteSheet->addFrame(frame.x, frame.y, frame.w, frame.h);
}
```

### 示例 3：混合规则和不规则

```cpp
// 第一行是规则网格
spriteSheet->addFrameGrid(0, 0, 64, 64, 8, 1);  // Frame 0-7

// 第二行是不规则布局
spriteSheet->addFrame(0, 64, 70, 80);           // Frame 8
spriteSheet->addFrame(75, 64, 65, 75);          // Frame 9
spriteSheet->addFrame(145, 64, 72, 78);         // Frame 10
```

---

## API 参考

### SpriteSheet

```cpp
// 构造
SpriteSheet(const std::string& texturePath);

// 添加帧
void addFrame(int x, int y, int w, int h);
void addFrameGrid(int startX, int startY, int frameWidth, int frameHeight,
                  int cols, int rows, int count = -1);

// 查询
const Frame& getFrame(size_t index) const;
size_t getFrameCount() const;
bool isValid() const;

// 纹理操作
void bind(GLuint unit = 0) const;
void unbind() const;
```

### Animation

```cpp
// 构造
Animation(const std::string& name, 
          const std::vector<int>& frames,
          float fps = 10.0f);

// 更新
void update(float deltaTime);

// 控制
void play();
void pause();
void reset();

// 设置
void setSpeed(float multiplier);
void setLooping(bool loop);
void setFPS(float fps);

// 查询
int getCurrentFrame() const;
bool isFinished() const;
bool isPaused() const;
const std::string& getName() const;
```

### SpriteRenderer

```cpp
// 构造
SpriteRenderer(SpriteSheet* sheet);

// 动画管理
void addAnimation(const Animation& anim);
bool setAnimation(const std::string& name);

// 更新和渲染
void update(float deltaTime);
void render(const glm::vec2& position,
            const glm::vec2& size,
            float rotation = 0.0f,
            const glm::vec3& color = glm::vec3(1.0f));

// 控制
void play();
void pause();

// 查询
Animation* getCurrentAnimation();
```

---

## 常见问题

### Q1: 精灵图加载失败怎么办？

**A**: 检查以下几点：
1. 文件路径是否正确（相对于可执行文件）
2. 图片格式是否支持（JPG, PNG, BMP 等）
3. OpenGL 上下文是否已创建
4. 查看控制台错误信息

```cpp
if (!spriteSheet->isValid()) {
    std::cerr << "Failed to load sprite sheet!" << std::endl;
    // 处理错误...
}
```

### Q2: 动画播放速度不对？

**A**: 检查以下几点：
1. FPS 设置是否正确
2. `deltaTime` 是否正确传入 `update()`
3. 速度倍率是否被修改

```cpp
// 查看当前设置
std::cout << "FPS: " << anim->getFPS() << std::endl;
std::cout << "Speed: " << anim->getSpeed() << std::endl;
```

### Q3: 精灵显示颠倒或翻转？

**A**: 纹理坐标系统问题。确保：
1. 使用 `stbi_set_flip_vertically_on_load(true)`（已在 Texture2 中设置）
2. SpriteSheet 已自动处理 Y 轴翻转

### Q4: 如何实现精灵翻转（水平/垂直）？

**A**: 有几种方法：

方法 1：调整渲染尺寸（简单）
```cpp
// 水平翻转
renderer->render(position, glm::vec2(-1.0f, 1.0f));

// 垂直翻转
renderer->render(position, glm::vec2(1.0f, -1.0f));
```

方法 2：在着色器中处理（高级）
- 添加 uniform 控制翻转
- 在顶点着色器中调整 UV 坐标

### Q5: 如何优化性能？

**A**: 性能优化建议：

1. **批量渲染**（未来扩展）
   - 一次调用渲染多个精灵
   - 使用实例化渲染

2. **纹理打包**
   - 将多个精灵图合并到纹理图集
   - 减少纹理切换

3. **剔除不可见精灵**
   ```cpp
   if (isVisible(sprite)) {
       renderer->render(...);
   }
   ```

4. **减少状态切换**
   - 按纹理分组渲染
   - 减少着色器切换

### Q6: 如何播放一次性动画（如攻击）？

**A**: 使用非循环动画：

```cpp
Sprite::Animation attackAnim("attack", {20, 21, 22, 23}, 15.0f);
attackAnim.setLooping(false);  // 不循环

// 播放
renderer->setAnimation("attack");

// 检查是否完成
if (renderer->getCurrentAnimation()->isFinished()) {
    // 动画完成，切换回 idle
    renderer->setAnimation("idle");
}
```

### Q7: 如何实现动画混合或过渡？

**A**: 当前系统不支持混合，但可以通过以下方式实现简单过渡：

```cpp
// 快速切换（无过渡）
renderer->setAnimation("walk");

// 延迟切换（等待当前动画播放完）
if (currentAnim->isFinished() || forceSwitch) {
    renderer->setAnimation("walk");
}
```

### Q8: 支持多层精灵吗？

**A**: 是的，创建多个渲染器：

```cpp
auto bodyRenderer = std::make_unique<Sprite::SpriteRenderer>(bodySheet.get());
auto weaponRenderer = std::make_unique<Sprite::SpriteRenderer>(weaponSheet.get());

// 渲染时分层
bodyRenderer->render(position, size);
weaponRenderer->render(position + offset, size);
```

---

## 高级主题

### 自定义着色器

如果需要自定义视觉效果，可以修改 `shaders/sprite/sprite.vert` 和 `sprite.frag`：

```glsl
// 示例：添加闪烁效果
uniform float uFlashIntensity;

void main() {
    vec4 texColor = texture(uTexture, TexCoord);
    vec3 flash = vec3(1.0) * uFlashIntensity;
    FragColor = vec4(uColor + flash, 1.0) * texColor;
}
```

### 事件系统

在动画特定帧触发事件：

```cpp
class AnimationWithEvents : public Sprite::Animation {
    std::map<int, std::function<void()>> frameEvents;
    
public:
    void addFrameEvent(int frame, std::function<void()> callback) {
        frameEvents[frame] = callback;
    }
    
    void update(float deltaTime) override {
        int prevFrame = getCurrentFrame();
        Animation::update(deltaTime);
        int currFrame = getCurrentFrame();
        
        if (prevFrame != currFrame && frameEvents.count(currFrame)) {
            frameEvents[currFrame]();  // 触发事件
        }
    }
};

// 使用
animWithEvents.addFrameEvent(2, []() {
    playSound("footstep.wav");  // 在第 2 帧播放脚步声
});
```

---

## 完整示例

参见 `src/01_Start/1_3_5_SpriteAnimation.cpp` 获取完整的工作示例。

运行示例：
```bash
cd build
./1_3_5_SpriteAnimation
```

---

## 资源推荐

### 精灵图工具

- **TexturePacker** - 专业的精灵图打包工具
- **Aseprite** - 像素艺术和动画编辑器
- **Piskel** - 免费在线像素编辑器
- **ShoeBox** - 免费精灵图工具

### 免费精灵图资源

- **OpenGameArt.org** - 开源游戏资源
- **Itch.io** - 独立游戏资源
- **Kenney.nl** - 免费游戏资源包

---

## 贡献和反馈

如果你发现 bug 或有改进建议，欢迎提交 Issue 或 Pull Request！

---

**版本**: 1.0.0  
**最后更新**: 2024年  
**作者**: OpenGL Learning Project

