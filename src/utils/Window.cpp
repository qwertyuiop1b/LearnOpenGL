#include "Window.h"
#include "GLFW/glfw3.h"


// static callback functions
void Window::keyCallbackWrapper(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Window* app = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (app) {
        app->checkIsRequestExit();
    }
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
    glfwSetFramebufferSizeCallback(mWindow, framebufferSizeCallbackWrapper);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD");
    }
}

void Window::checkIsRequestExit() {
    if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(mWindow, true);
    }
}

