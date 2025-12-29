# Input System 使用指南

## 概述

Input系统是一个完全解耦的输入管理模块，使用单例模式设计，支持键盘、鼠标和输入历史记录功能。

## 特性

- ✅ 单例模式访问：全局统一的输入状态管理
- ✅ 键盘输入：支持按下、释放、持续按住检测
- ✅ 鼠标按键：支持左键、右键、中键等所有按键
- ✅ 鼠标位置：实时位置和移动增量
- ✅ 滚轮支持：获取每帧的滚轮增量
- ✅ 输入历史：记录最近1000条输入事件用于调试和回放

## 基础使用

### 键盘输入检测

```cpp
#include "utils/Input.h"

// 检查键是否被按住（每帧都会返回true）
if (Input::getInstance().GetKey(GLFW_KEY_W)) {
    camera.moveForward(deltaTime);
}

// 检查键是否刚被按下（只在按下的那一帧返回true）
if (Input::getInstance().GetKeyDown(GLFW_KEY_SPACE)) {
    player.jump();
}

// 检查键是否刚被释放（只在释放的那一帧返回true）
if (Input::getInstance().GetKeyUp(GLFW_KEY_SHIFT)) {
    player.stopSprinting();
}
```

### 鼠标按键检测

```cpp
// 检查鼠标左键是否按住
if (Input::getInstance().GetMouseButton(GLFW_MOUSE_BUTTON_LEFT)) {
    // 持续拖拽
}

// 检查鼠标右键刚按下
if (Input::getInstance().GetMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT)) {
    // 右键点击事件
}

// 检查鼠标中键刚释放
if (Input::getInstance().GetMouseButtonUp(GLFW_MOUSE_BUTTON_MIDDLE)) {
    // 中键释放事件
}
```

### 鼠标位置和移动

```cpp
// 获取当前鼠标位置
glm::vec2 mousePos = Input::getInstance().GetMousePosition();
std::cout << "Mouse at: " << mousePos.x << ", " << mousePos.y << std::endl;

// 获取鼠标移动增量（与上一帧的差值）
glm::vec2 mouseDelta = Input::getInstance().GetMouseDelta();
camera.rotate(mouseDelta.x * sensitivity, mouseDelta.y * sensitivity);

// 检查是否是第一次移动（避免大幅跳跃）
if (!Input::getInstance().IsFirstMouse()) {
    // 处理鼠标移动
}
```

### 滚轮输入

```cpp
// 获取滚轮增量（每帧自动重置为0）
glm::vec2 scroll = Input::getInstance().GetScrollDelta();
if (scroll.y != 0) {
    camera.zoom(scroll.y);
}
```

## 高级功能

### 输入历史记录

```cpp
// 获取输入历史（最多1000条）
const auto& history = Input::getInstance().GetInputHistory();

for (const auto& event : history) {
    std::cout << "Event at " << event.timestamp << ": ";
    
    switch (event.type) {
        case InputEventType::KEY_DOWN:
            std::cout << "Key Down: " << event.code << std::endl;
            break;
        case InputEventType::KEY_UP:
            std::cout << "Key Up: " << event.code << std::endl;
            break;
        case InputEventType::MOUSE_BUTTON_DOWN:
            std::cout << "Mouse Button Down: " << event.code 
                      << " at (" << event.position.x << ", " << event.position.y << ")" << std::endl;
            break;
        case InputEventType::MOUSE_MOVE:
            std::cout << "Mouse Move: delta(" << event.delta.x << ", " << event.delta.y << ")" << std::endl;
            break;
        case InputEventType::SCROLL:
            std::cout << "Scroll: (" << event.delta.x << ", " << event.delta.y << ")" << std::endl;
            break;
    }
}

// 清空历史记录
Input::getInstance().ClearHistory();
```

## 完整示例

```cpp
#include "utils/Window.h"
#include "utils/Input.h"
#include "utils/Camera.h"

int main() {
    Window window(800, 600, "Input System Example");
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
    
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    
    while (!window.shouldClose()) {
        // 计算deltaTime
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // 键盘控制相机移动
        float cameraSpeed = 2.5f * deltaTime;
        if (Input::getInstance().GetKey(GLFW_KEY_W)) {
            camera.moveForward(cameraSpeed);
        }
        if (Input::getInstance().GetKey(GLFW_KEY_S)) {
            camera.moveBackward(cameraSpeed);
        }
        if (Input::getInstance().GetKey(GLFW_KEY_A)) {
            camera.moveLeft(cameraSpeed);
        }
        if (Input::getInstance().GetKey(GLFW_KEY_D)) {
            camera.moveRight(cameraSpeed);
        }
        
        // 鼠标控制相机旋转
        glm::vec2 mouseDelta = Input::getInstance().GetMouseDelta();
        float sensitivity = 0.1f;
        camera.rotate(mouseDelta.x * sensitivity, -mouseDelta.y * sensitivity);
        
        // 滚轮控制视野缩放
        glm::vec2 scroll = Input::getInstance().GetScrollDelta();
        if (scroll.y != 0) {
            camera.zoom(scroll.y);
        }
        
        // ESC键退出（自动处理，但也可以手动检测）
        if (Input::getInstance().GetKeyDown(GLFW_KEY_ESCAPE)) {
            break;
        }
        
        // 渲染...
        
        window.swapBuffer();
        window.pollEvents();
    }
    
    return 0;
}
```

## 注意事项

1. **Input::Update()自动调用**：已在`Application::update()`或`Window`类中自动调用，无需手动调用
2. **状态更新时机**：在`glfwPollEvents()`之后、下一帧渲染之前自动更新
3. **首次鼠标移动**：系统会自动处理首次鼠标移动，避免大幅跳跃
4. **滚轮增量重置**：每帧自动重置为(0,0)，确保只在滚动时有值
5. **线程安全**：单例实现使用静态局部变量，保证线程安全的初始化

## 常见GLFW键码

```cpp
// 字母键
GLFW_KEY_A ~ GLFW_KEY_Z

// 数字键
GLFW_KEY_0 ~ GLFW_KEY_9

// 功能键
GLFW_KEY_SPACE
GLFW_KEY_ESCAPE
GLFW_KEY_ENTER
GLFW_KEY_TAB
GLFW_KEY_BACKSPACE
GLFW_KEY_LEFT_SHIFT, GLFW_KEY_RIGHT_SHIFT
GLFW_KEY_LEFT_CONTROL, GLFW_KEY_RIGHT_CONTROL
GLFW_KEY_LEFT_ALT, GLFW_KEY_RIGHT_ALT

// 方向键
GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_LEFT, GLFW_KEY_RIGHT

// 鼠标按键
GLFW_MOUSE_BUTTON_LEFT    // 0
GLFW_MOUSE_BUTTON_RIGHT   // 1
GLFW_MOUSE_BUTTON_MIDDLE  // 2
```

## 架构说明

Input系统完全解耦于Window和Application，通过GLFW回调机制接收输入事件：

```
GLFW输入事件 → Window回调 → Input::OnXxxCallback() → Input内部状态更新
                                                             ↓
游戏逻辑 ← Input::GetXxx() ← Input::Update() ← Application::update()
```

这种设计确保了：
- 输入逻辑与窗口管理分离
- 方便单元测试和输入回放
- 统一的输入查询接口
- 可扩展的输入历史系统


