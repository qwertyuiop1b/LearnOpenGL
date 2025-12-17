#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "utils/FPS.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {

    glViewport(0, 0, width, height);
    // main rendering --> Window Resize Draw Again
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
}


void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow* window = glfwCreateWindow(800, 600, "1_0_1_Window", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create glfw window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // 0 = 禁用V-sync, 1 = 启用V-Sync(默认), 跟随显示器的频率，等待显示器垂直刷新信号再交换前后缓冲区，防止画面撕裂 
    glfwSwapInterval(0); 

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glViewport(0, 0, 800, 600);

    const auto frameRate = 60;
    FPS fps(frameRate);
    while (!glfwWindowShouldClose(window)) {
        // handle input
        process_input(window);

        // rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
        fps.update();

        static int frameCounter = 0;
        if (++frameCounter % frameRate == 0) {
            std::cout << "DeltaTime: " << (fps.getDeltaTime() * 1000.f) << "ms" << std::endl;
        }
    }
    glfwTerminate();
    return 0;
}