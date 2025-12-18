#ifndef OPENGL_UTILS_INPUT_H
#define OPENGL_UTILS_INPUT_H

#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

enum class InputEventType {
    KEY_DOWN,
    KEY_UP,
    MOUSE_BUTTON_DOWN,
    MOUSE_BUTTON_UP,
    MOUSE_MOVE,
    SCROLL
};

struct InputEvent {
    double timestamp;
    InputEventType type;
    int code;  // key code or mouse button code
    glm::vec2 position;  // for mouse events
    glm::vec2 delta;     // for mouse move or scroll events
};

class Input {
public:
    // 禁止拷贝和赋值
    Input(const Input&) = delete;
    Input& operator=(const Input&) = delete;

    // 获取单例实例
    static Input& getInstance();

    // 系统更新方法（每帧调用）
    void Update();

    // 键盘输入接口
    bool GetKey(int key) const;
    bool GetKeyDown(int key) const;
    bool GetKeyUp(int key) const;

    // 鼠标按键接口
    bool GetMouseButton(int button) const;
    bool GetMouseButtonDown(int button) const;
    bool GetMouseButtonUp(int button) const;

    // 鼠标位置接口
    glm::vec2 GetMousePosition() const;
    glm::vec2 GetMouseDelta();
    bool IsFirstMouse() const;

    // 滚轮接口
    glm::vec2 GetScrollDelta() const;

    // 输入历史记录接口
    const std::vector<InputEvent>& GetInputHistory() const;
    void ClearHistory();

    // GLFW回调方法（供Window类调用）
    void OnKeyCallback(int key, int scancode, int action, int mods);
    void OnMouseButtonCallback(int button, int action, int mods);
    void OnCursorPosCallback(double xpos, double ypos);
    void OnScrollCallback(double xoffset, double yoffset);

private:
    // 私有构造函数（单例模式）
    Input();
    ~Input() = default;

    // 键盘状态（双缓冲）
    std::unordered_map<int, bool> mCurrentKeys;
    std::unordered_map<int, bool> mPreviousKeys;

    // 鼠标按键状态（双缓冲）
    std::unordered_map<int, bool> mCurrentMouseButtons;
    std::unordered_map<int, bool> mPreviousMouseButtons;

    // 鼠标位置
    glm::vec2 mCurrentMousePos;
    glm::vec2 mLastMousePos;
    bool mFirstMouse;

    // 滚轮增量
    glm::vec2 mScrollDelta;

    // 输入历史记录
    std::vector<InputEvent> mInputHistory;
    static constexpr size_t MAX_HISTORY_SIZE = 1000;

    // 辅助方法
    void AddToHistory(InputEventType type, int code, glm::vec2 position = glm::vec2(0.0f), glm::vec2 delta = glm::vec2(0.0f));
    double GetCurrentTime() const;
};

#endif

