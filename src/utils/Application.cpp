#include "Application.h"
#include "GLFW/glfw3.h"
#include <assert.h>
#include <stdexcept>



Application::Application(unsigned int width, unsigned int height, const std::string& title)
    : mWidth(width), mHeight(height), mTitle(title), mWindow(nullptr) {

}

Application::~Application() {
    cleanup();
}

void Application::init(unsigned int glVersionMajor, unsigned int glVersionMinor) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glVersionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glVersionMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    mWindow = glfwCreateWindow(mWidth, mHeight, mTitle.c_str(), nullptr, nullptr);
    if (mWindow == nullptr) {
        cleanup();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(mWindow);
    // Set the window pointer for callbacks
    glfwSetWindowUserPointer(mWindow, this);
    glfwSetKeyCallback(mWindow, keyCallbackWrapper);
    glfwSetFramebufferSizeCallback(mWindow, framebufferSizeCallbackWrapper);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cleanup();
        throw std::runtime_error("Failed to initialize GLAD");
    }
}

void Application::run() {
    while(!glfwWindowShouldClose(mWindow)) {
        render();
        update();
    }
}

void Application::requestExit() {
    glfwSetWindowShouldClose(mWindow, true);
}

void Application::render() {}

void Application::checkIsRequestExit() {
    if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(mWindow, true);
    }
}

void Application::update() {
    assert(mWindow != nullptr && "Window is not initialized");
    glfwSwapBuffers(mWindow);
    glfwPollEvents();
}

void Application::cleanup() {
    if (mWindow) {
        glfwDestroyWindow(mWindow);
        mWindow = nullptr;
    }
    glfwTerminate();
}


// static callback functions
void Application::keyCallbackWrapper(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (app) {
        app->checkIsRequestExit();
    }
}

void Application::framebufferSizeCallbackWrapper(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (app) {
        app->mWidth = width;
        app->mHeight = height;
    }
}