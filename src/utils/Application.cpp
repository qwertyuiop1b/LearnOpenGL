#include "Application.h"

#include <assert.h>
#include <stdexcept>
#include <iostream>


Application::Application(unsigned int width, unsigned int height, const std::string& title)
    : mWidth(width), mHeight(height), mTitle(title), mWindow(nullptr), mIsRunning(false) {

}

void Application::init(unsigned int glVersionMajor, unsigned int glVersionMinor) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glVersionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glVersionMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    mWindow = glfwCreateWindow(mWidth, mHeight, mTitle.c_str(), nullptr, nullptr);
    if (mWindow == nullptr) {
        cleanup();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(mWindow);
    // Set the window pointer for callbacks
    glfwSetWindowUserPointer(mWindow, this);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cleanup();
        throw std::runtime_error("Failed to initialize GLAD");
    }
    mIsRunning = true;
}

void Application::run() {
    while(mIsRunning) {
        render();
        update();
    }
}

void Application::render() {

}

void Application::checkIsRunning() {
    if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(mWindow, true);
        requestExit();
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
        std::cout << "Get Application Instance Successfully" << std::endl;
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