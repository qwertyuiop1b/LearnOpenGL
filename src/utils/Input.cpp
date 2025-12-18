#include "Input.h"
#include <algorithm>

Input::Input()
    : mCurrentMousePos(0.0f, 0.0f)
    , mLastMousePos(0.0f, 0.0f)
    , mFirstMouse(true)
    , mScrollDelta(0.0f, 0.0f) {
}

Input& Input::getInstance() {
    static Input instance;
    return instance;
}

void Input::Update() {
    // 复制当前状态到上一帧状态
    mPreviousKeys = mCurrentKeys;
    mPreviousMouseButtons = mCurrentMouseButtons;
    
    // 更新鼠标位置历史
    mLastMousePos = mCurrentMousePos;
    
    // 重置滚轮增量（滚轮是瞬时事件）
    mScrollDelta = glm::vec2(0.0f, 0.0f);
}

bool Input::GetKey(int key) const {
    auto it = mCurrentKeys.find(key);
    return it != mCurrentKeys.end() && it->second;
}

bool Input::GetKeyDown(int key) const {
    auto currentIt = mCurrentKeys.find(key);
    auto previousIt = mPreviousKeys.find(key);
    
    bool currentPressed = (currentIt != mCurrentKeys.end() && currentIt->second);
    bool previousPressed = (previousIt != mPreviousKeys.end() && previousIt->second);
    
    return currentPressed && !previousPressed;
}

bool Input::GetKeyUp(int key) const {
    auto currentIt = mCurrentKeys.find(key);
    auto previousIt = mPreviousKeys.find(key);
    
    bool currentPressed = (currentIt != mCurrentKeys.end() && currentIt->second);
    bool previousPressed = (previousIt != mPreviousKeys.end() && previousIt->second);
    
    return !currentPressed && previousPressed;
}

bool Input::GetMouseButton(int button) const {
    auto it = mCurrentMouseButtons.find(button);
    return it != mCurrentMouseButtons.end() && it->second;
}

bool Input::GetMouseButtonDown(int button) const {
    auto currentIt = mCurrentMouseButtons.find(button);
    auto previousIt = mPreviousMouseButtons.find(button);
    
    bool currentPressed = (currentIt != mCurrentMouseButtons.end() && currentIt->second);
    bool previousPressed = (previousIt != mPreviousMouseButtons.end() && previousIt->second);
    
    return currentPressed && !previousPressed;
}

bool Input::GetMouseButtonUp(int button) const {
    auto currentIt = mCurrentMouseButtons.find(button);
    auto previousIt = mPreviousMouseButtons.find(button);
    
    bool currentPressed = (currentIt != mCurrentMouseButtons.end() && currentIt->second);
    bool previousPressed = (previousIt != mPreviousMouseButtons.end() && previousIt->second);
    
    return !currentPressed && previousPressed;
}

glm::vec2 Input::GetMousePosition() const {
    return mCurrentMousePos;
}

glm::vec2 Input::GetMouseDelta() {
    if (mFirstMouse) {
        mLastMousePos = mCurrentMousePos;
        mFirstMouse = false;
        return glm::vec2(0.0f, 0.0f);
    }
    return mCurrentMousePos - mLastMousePos;
}

bool Input::IsFirstMouse() const {
    return mFirstMouse;
}

glm::vec2 Input::GetScrollDelta() const {
    return mScrollDelta;
}

const std::vector<InputEvent>& Input::GetInputHistory() const {
    return mInputHistory;
}

void Input::ClearHistory() {
    mInputHistory.clear();
}

void Input::OnKeyCallback(int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        mCurrentKeys[key] = true;
        AddToHistory(InputEventType::KEY_DOWN, key);
    } else if (action == GLFW_RELEASE) {
        mCurrentKeys[key] = false;
        AddToHistory(InputEventType::KEY_UP, key);
    }
    // GLFW_REPEAT 不改变状态，保持为true
}

void Input::OnMouseButtonCallback(int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        mCurrentMouseButtons[button] = true;
        AddToHistory(InputEventType::MOUSE_BUTTON_DOWN, button, mCurrentMousePos);
    } else if (action == GLFW_RELEASE) {
        mCurrentMouseButtons[button] = false;
        AddToHistory(InputEventType::MOUSE_BUTTON_UP, button, mCurrentMousePos);
    }
}

void Input::OnCursorPosCallback(double xpos, double ypos) {
    glm::vec2 oldPos = mCurrentMousePos;
    mCurrentMousePos = glm::vec2(static_cast<float>(xpos), static_cast<float>(ypos));
    
    glm::vec2 delta = mCurrentMousePos - oldPos;
    AddToHistory(InputEventType::MOUSE_MOVE, 0, mCurrentMousePos, delta);
}

void Input::OnScrollCallback(double xoffset, double yoffset) {
    mScrollDelta = glm::vec2(static_cast<float>(xoffset), static_cast<float>(yoffset));
    AddToHistory(InputEventType::SCROLL, 0, glm::vec2(0.0f), mScrollDelta);
}

void Input::AddToHistory(InputEventType type, int code, glm::vec2 position, glm::vec2 delta) {
    InputEvent event;
    event.timestamp = GetCurrentTime();
    event.type = type;
    event.code = code;
    event.position = position;
    event.delta = delta;
    
    mInputHistory.push_back(event);
    
    // 限制历史记录大小
    if (mInputHistory.size() > MAX_HISTORY_SIZE) {
        mInputHistory.erase(mInputHistory.begin());
    }
}

double Input::GetCurrentTime() const {
    return glfwGetTime();
}

