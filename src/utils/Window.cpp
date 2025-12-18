#include "Window.h"
#include "Input.h"
#include "GLFW/glfw3.h"


// static callback functions
void Window::keyCallbackWrapper(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Input::getInstance().OnKeyCallback(key, scancode, action, mods);
}

void Window::mouseButtonCallbackWrapper(GLFWwindow* window, int button, int action, int mods) {
    Input::getInstance().OnMouseButtonCallback(button, action, mods);
}

void Window::cursorPosCallbackWrapper(GLFWwindow* window, double xpos, double ypos) {
    Input::getInstance().OnCursorPosCallback(xpos, ypos);
}

void Window::scrollCallbackWrapper(GLFWwindow* window, double xoffset, double yoffset) {
    Input::getInstance().OnScrollCallback(xoffset, yoffset);
}

void Window::framebufferSizeCallbackWrapper(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    Window* app = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (app) {
        app->mWidth = width;
        app->mHeight = height;
    }
}


Window::Window(unsigned int width, unsigned int height, const std::string& title)
    : mWidth(width), mHeight(height), mTitle(title), mWindow(nullptr) {
        init(3, 3);
}

Window::~Window() {
    glfwTerminate();
}


void Window::init(unsigned int glVersionMajor, unsigned int glVersionMinor) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glVersionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glVersionMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    mWindow = glfwCreateWindow(mWidth, mHeight, mTitle.c_str(), nullptr, nullptr);
    if (mWindow == nullptr) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(mWindow);
    glfwSwapInterval(0);
    // Set the window pointer for callbacks
    glfwSetWindowUserPointer(mWindow, this);
    glfwSetKeyCallback(mWindow, keyCallbackWrapper);
    glfwSetMouseButtonCallback(mWindow, mouseButtonCallbackWrapper);
    glfwSetCursorPosCallback(mWindow, cursorPosCallbackWrapper);
    glfwSetScrollCallback(mWindow, scrollCallbackWrapper);
    glfwSetFramebufferSizeCallback(mWindow, framebufferSizeCallbackWrapper);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD");
    }
}
