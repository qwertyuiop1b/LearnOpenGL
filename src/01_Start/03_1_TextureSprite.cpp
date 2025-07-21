#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "utils/Shader.h"
#include "utils/Utils.h"


const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;



int main() {  
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "03_TextureSprite", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create glfw window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, Utils::framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Shader shader("shaders/")

    return 0;
}