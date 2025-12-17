# 如何运行火焰 Sprite 动画

## ✅ 问题已修复

程序现在可以从任何目录运行，会自动检测资源文件路径！

## 🚀 运行方法

### 方法 1：使用启动脚本（推荐）

```bash
# 从项目根目录
./run_fire_animation.sh
```

### 方法 2：从项目根目录运行

```bash
./build/1_3_5_SpriteAnimation
```

### 方法 3：从 build 目录运行

```bash
cd build
./1_3_5_SpriteAnimation
```

## 📋 确认清单

运行前确保：

- ✅ 已编译程序
  ```bash
  cd build
  make 1_3_5_SpriteAnimation
  ```

- ✅ 纹理文件存在
  ```bash
  ls -lh textures/fire_frame.jpg
  # 应显示：-rw-rw-r-- 1 hhq hhq 404K ... textures/fire_frame.jpg
  ```

- ✅ 着色器文件存在
  ```bash
  ls shaders/sprite/
  # 应显示：sprite.frag  sprite.vert
  ```

## 🎮 控制说明

### ImGui 控制面板

**动画切换**:
- `Fire (36 frames)` - 完整火焰动画（所有 36 帧，15 FPS）
- `Fire Fast (18 frames)` - 快速火焰（跳帧，20 FPS）

**速度控制**:
- 拖动 `Speed` 滑块：0.1x - 5.0x
- 1.0x = 正常速度
- 2.0x = 两倍速
- 0.5x = 半速（慢动作）

**播放控制**:
- `Play` - 播放动画
- `Pause` - 暂停动画
- `Reset` - 重置到第一帧
- `Loop` 复选框 - 切换循环播放

### 键盘快捷键
- `ESC` - 退出程序
- `Ctrl+C` - 终止程序（终端）

## 🔧 故障排除

### 问题 1：找不到纹理文件

**症状**:
```
Failed to load texture: ../textures/fire_frame.jpg
STB Error: can't fopen
```

**解决**:
1. 确认从正确的目录运行
2. 检查 `textures/fire_frame.jpg` 是否存在
3. 使用启动脚本 `./run_fire_animation.sh`

### 问题 2：程序立即崩溃

**症状**:
```
Segmentation fault
```

**可能原因**:
- VBO 引用丢失（已修复）
- 着色器加载失败
- OpenGL 上下文问题

**解决**:
1. 确保从终端运行（不是后台）
2. 检查 OpenGL 驱动是否正常
3. 查看完整的错误输出

### 问题 3：着色器编译错误

**症状**:
```
ERROR: program link error
```

**解决**:
1. 确认 `shaders/sprite/` 目录存在
2. 检查着色器文件内容完整
3. 查看详细的编译错误信息

### 问题 4：窗口黑屏或白屏

**可能原因**:
- 纹理未正确绑定
- 着色器 uniform 未设置
- 投影矩阵不正确

**调试**:
- 查看控制台输出，确认所有初始化成功
- 检查是否有 OpenGL 错误

## 📊 性能

**预期性能**:
- **FPS**: 60 FPS（V-Sync 开启）或更高
- **内存**: ~50MB
- **GPU 负载**: 极低（单个四边形渲染）

**系统要求**:
- OpenGL 3.3+
- 支持 GLSL 330

## 🎯 成功运行的标志

运行成功时，你应该看到：

```
=== Sprite Animation Demo ===
Initializing...
Found texture at: textures/fire_frame.jpg
Path: fire_frame.jpg
LoadTexture From Path: textures/fire_frame.jpg
Texture loaded successfully: textures/fire_frame.jpg (1920x1920, 3 channels)
Sprite sheet loaded: textures/fire_frame.jpg (1920x1920)
Defining frames...
Frame 0 added: pos(0,0) size(320x320) UV(...)
...
Frame 35 added: pos(1600,1600) size(320x320) UV(...)
Added 36 frames in 6x6 grid
Fire sprite sheet: 1920x1920, 6x6 grid, 320x320 per frame
Total frames defined: 36
Loading shaders:
  Vertex: shaders/sprite/sprite.vert
  Fragment: shaders/sprite/sprite.frag
Sprite render data initialized
SpriteRenderer created
Creating animations...
Animation 'fire' added (36 frames)
Animation 'fire_fast' added (18 frames)
Switched to animation: fire
Initialization complete!

Controls:
- Use ImGui panel to control animation
- Switch between Fire (36 frames) and Fire Fast (18 frames)
- Adjust speed (0.1x - 5.0x)
- Toggle loop and pause/play
============================
```

然后窗口打开，显示火焰动画！

## 📚 更多信息

- 完整文档：`SPRITE_ANIMATION_GUIDE.md`
- 火焰动画说明：`FIRE_ANIMATION_README.md`
- Texture2 使用指南：`TEXTURE2_USAGE_EXAMPLES.md`

## 🐛 报告问题

如果遇到其他问题，请提供：
1. 完整的控制台输出
2. 运行命令
3. 当前工作目录（`pwd`）
4. 系统信息（`glxinfo | grep "OpenGL version"`）

---

**祝你使用愉快！** 🔥✨

