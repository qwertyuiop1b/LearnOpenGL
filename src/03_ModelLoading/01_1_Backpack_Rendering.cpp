#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"
#include "utils/Camera.h"
#include "utils/Model.h"
#include "utils/OribitCamera.h"
#include "utils/Texture.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const int width = 800;
const int height = 600;
const char *title = "Backpack Rendering";


bool dragging = false;
double lastX = width / 2.0, lastY = height / 2.0;
double curX = width / 2.0, curY = height / 2.0;

OribitCamera oribitCamera(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.0f, 0.f), 6.f, 1.f, 0, 0);


void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    oribitCamera.zoom(static_cast<float>(yoffset));
}


int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create glfw window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetScrollCallback(window, scrollCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to load gl" << std::endl;
        glfwTerminate();
        return -1;
    }

    Camera camera(glm::vec3(0.f, 0.f, 6.f));
    Shader shader("shaders/03_shaders/01_1_Backpack_Rendering_vs.glsl", "shaders/03_shaders/01_1_Backpack_Rendering_fs.glsl");
    Model modelObj("models/backpack/backpack.obj");
    Texture texture;

    glEnable(GL_DEPTH_TEST);
    while(!glfwWindowShouldClose(window)) {
        // Handle input
        const auto leftMouseBtnState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        if (leftMouseBtnState == GLFW_PRESS) {
            if (!dragging) {
                glfwGetCursorPos(window, &lastX, &lastY);
                dragging = true;
            }
            glfwGetCursorPos(window, &curX, &curY);
            const auto deltaX = curX - lastX;
            const auto deltaY = curY - lastY;
            oribitCamera.rotateAzimuth(glm::radians(static_cast<float>(deltaX) * 0.5f));
            oribitCamera.rotatePolar(glm::radians(static_cast<float>(deltaY) * 0.5f));
            lastX = curX;
            lastY = curY;
        } else {
            dragging = false;
        }

        glViewport(0, 0, width, height);
        glClearColor(0.01f, 0.01f, 0.01f, 0.1f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        // glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 100.f);
        glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)width / (float)height, 0.1f, 100.f);
        glm::mat4 view = oribitCamera.getViewMatrix();
        shader.setMatrix4("projection", projection);
        shader.setMatrix4("view", view);
        glm::mat4 model = glm::mat4(1.f);
        model = glm::translate(model, glm::vec3(0.f, 0.f, 0.f));
        model = glm::scale(model, glm::vec3(1.f));
        model = glm::rotate(model, glm::radians(30.f), glm::vec3(0.f, 1.0f, 0.f));
        shader.setMatrix4("model", model);

        modelObj.draw(shader);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}