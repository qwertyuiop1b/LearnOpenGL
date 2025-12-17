# 如何准备 Sprite Sheet（精灵图）

## 快速开始

Sprite 动画示例程序需要一个精灵图文件：`textures/sprite_sheet.png`

## 方法 1：使用示例精灵图（推荐）

你可以从以下免费资源网站下载现成的精灵图：

### 免费资源网站

1. **OpenGameArt.org**
   - 网址：https://opengameart.org
   - 搜索："sprite sheet character" 或 "2d character sprite"
   - 许可：大多数是 CC0（公共领域）

2. **Kenney.nl**
   - 网址：https://kenney.nl/assets
   - 提供大量免费游戏资源包
   - 许可：CC0

3. **Itch.io**
   - 网址：https://itch.io/game-assets/free
   - 有很多独立艺术家分享的资源

### 示例精灵图布局

程序期望的布局（你可以修改代码以适应其他布局）：

```
布局示例（每帧 64x64 像素）：
┌─────────────────────────────┐
│ [0] [1] [2] [3]             │  Row 0: Idle 动画（4 帧）
│ [4] [5] [6] [7] [8] [9]     │  Row 1: Walk 动画（6 帧）
└─────────────────────────────┘

纹理尺寸：384x128 像素
帧尺寸：64x64 像素
```

## 方法 2：创建自己的精灵图

### 工具推荐

1. **Aseprite**（付费，约 $20）
   - 最专业的像素艺术和动画工具
   - 支持导出精灵图

2. **Piskel**（免费，在线）
   - 网址：https://www.piskelapp.com
   - 在线像素编辑器
   - 可以导出精灵图

3. **LibreSprite**（免费）
   - Aseprite 的免费开源版本
   - 功能类似 Aseprite

### 创建步骤

1. 在工具中创建动画
2. 设置每帧尺寸（推荐 64x64 或 32x32）
3. 导出为精灵图（Sprite Sheet）
4. 保存为 PNG 格式

## 方法 3：使用现有图片创建简单精灵图

如果你只是测试，可以用 GIMP、Photoshop 或任何图像编辑器：

### GIMP 示例

1. 创建新图像（例如 256x64 像素）
2. 创建 4 个 64x64 的区域
3. 在每个区域画不同的图案或文字
4. 导出为 PNG

### 最简单的测试图

使用 Python + PIL 快速生成：

```python
from PIL import Image, ImageDraw, ImageFont

# 创建 256x64 的精灵图
img = Image.new('RGBA', (256, 64), (0, 0, 0, 0))
draw = ImageDraw.Draw(img)

# 绘制 4 帧
colors = [(255, 0, 0, 255), (0, 255, 0, 255), (0, 0, 255, 255), (255, 255, 0, 255)]
for i in range(4):
    x = i * 64
    draw.rectangle([x, 0, x+64, 64], fill=colors[i], outline=(255,255,255,255))
    draw.text((x+20, 20), f"Frame {i}", fill=(255,255,255,255))

img.save('sprite_sheet.png')
```

## 方法 4：使用测试纹理

如果你没有精灵图，程序会尝试加载但会显示警告。你可以：

1. 使用项目中已有的任何纹理（如 `container.jpg`）
2. 修改代码路径：
   ```cpp
   spriteSheet = std::make_unique<Sprite::SpriteSheet>("textures/container.jpg");
   ```
3. 调整帧定义以适应新纹理

## 修改代码以适应你的精灵图

在 `src/01_Start/1_3_5_SpriteAnimation.cpp` 的 `init()` 方法中：

```cpp
// 1. 修改纹理路径
spriteSheet = std::make_unique<Sprite::SpriteSheet>("textures/your_sprite.png");

// 2. 根据你的精灵图布局定义帧
// 方式 A：手动定义（不规则布局）
spriteSheet->addFrame(x, y, width, height);  // 添加每一帧

// 方式 B：网格定义（规则布局）
spriteSheet->addFrameGrid(
    0, 0,       // 起始位置 (x, y)
    64, 64,     // 每帧大小 (width, height)
    8, 2,       // 网格尺寸 (cols, rows)
    -1          // 帧数量（-1 = 全部）
);

// 3. 定义动画时使用正确的帧索引
Animation idleAnim("idle", {0, 1, 2, 3}, 10.0f);  // 使用帧 0-3
```

## 常见布局示例

### 水平条带（最常见）
```
[0][1][2][3][4][5][6][7]  ← 一行，多帧
```

代码：
```cpp
spriteSheet->addFrameGrid(0, 0, frameWidth, frameHeight, 8, 1);
```

### 网格布局
```
[0][1][2][3]
[4][5][6][7]
[8][9][10][11]
```

代码：
```cpp
spriteSheet->addFrameGrid(0, 0, frameWidth, frameHeight, 4, 3);
```

### 混合布局
```
[0][1][2][3]        ← Idle
[4][5][6][7][8][9]  ← Walk
```

代码：
```cpp
spriteSheet->addFrame(0, 0, 64, 64);    // Frame 0
spriteSheet->addFrame(64, 0, 64, 64);   // Frame 1
// ... 手动添加每一帧
```

## 推荐的精灵图规格

- **尺寸**：64x64 或 32x32 像素每帧
- **格式**：PNG（支持透明度）
- **布局**：规则网格（易于定义）
- **动画**：
  - Idle：3-4 帧
  - Walk/Run：6-8 帧
  - Jump：4-6 帧
  - Attack：3-5 帧

## 故障排除

### 问题：纹理加载失败

1. 检查文件路径是否正确
2. 确认文件格式（JPG, PNG, BMP）
3. 查看控制台错误信息

### 问题：动画显示不正确

1. 验证帧定义是否匹配精灵图布局
2. 检查 UV 坐标计算
3. 确认帧索引在动画中正确

### 问题：精灵显示颠倒

- 已在代码中自动处理 Y 轴翻转
- 如果仍有问题，检查 `stbi_set_flip_vertically_on_load`

## 更多帮助

参见 `SPRITE_ANIMATION_GUIDE.md` 获取完整的 API 文档和使用指南。

---

**祝你游戏开发愉快！** 🎮✨

