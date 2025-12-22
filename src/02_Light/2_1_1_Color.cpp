

#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include "utils/Window.h"
#include "utils/FPS.h"
#include "utils/Input.h"
#include <utils/Shader.h>
#include "utils/OribitCamera.h"
#include "utils/VertexArray.h"
#include "utils/VertexBuffer.h"



class ColorChapter {
public:
    ColorChapter(unsigned int width, unsigned int height, const std::string& title) 
    : window(std::make_unique<Window>(width, height, title))
    , containerShader(std::make_unique<Shader>("shaders/02_shaders/2_1_1_Color.vert", "shaders/02_shaders/2_1_1_Color.frag"))
    , lightShader(std::make_unique<Shader>("shaders/02_shaders/2_1_1_Light.vert", "shaders/02_shaders/2_1_1_Light.frag"))
    , containerVAO(std::make_unique<VertexArray>())
    , lightVAO(std::make_unique<VertexArray>())
    , oribitCamera(std::make_unique<OribitCamera>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 10.0f, 0.1f, 0.0f, 0.0f)) {
        init();
    }

    void run() {
        FPS fps(120);
        glEnable(GL_DEPTH_TEST);
        glm::vec3 objectColor(1.0f, 0.5f, 0.31f);
        glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
        Input& input = Input::getInstance();

        lastX = window->getWidth() / 2.0;
        lastY = window->getHeight() / 2.0;

        while (!window->shouldClose()) {
            window->pollEvents();
            fps.update();

            const auto leftMouseBtnState = glfwGetMouseButton(window->getGLFWWindow(), GLFW_MOUSE_BUTTON_LEFT);
            if (leftMouseBtnState == GLFW_PRESS) {
                if (!dragging) {
                    glfwGetCursorPos(window->getGLFWWindow(), &lastX, &lastY);
                    dragging = true;
                }
                glfwGetCursorPos(window->getGLFWWindow(), &curX, &curY);
                const auto deltaX = curX - lastX;
                const auto deltaY = curY - lastY;
                oribitCamera->rotateAzimuth(glm::radians(static_cast<float>(deltaX) * 0.5f));
                oribitCamera->rotatePolar(glm::radians(static_cast<float>(deltaY) * 0.5f));
                lastX = curX;
                lastY = curY;
            } else {
                dragging = false;
            }

            glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(window->getWidth()) / static_cast<float>(window->getHeight()), 0.1f, 100.0f);
            containerShader->use();
            containerShader->setFloat3("lightColor", lightColor);
            containerShader->setFloat3("objectColor", objectColor);
            containerShader->setMatrix4("model", glm::mat4(1.0f));
            containerShader->setMatrix4("view", oribitCamera->getViewMatrix());
            containerShader->setMatrix4("projection", projection);
            containerVAO->bind();
            glDrawArrays(GL_TRIANGLES, 0, 36);
            containerVAO->unbind();

            lightShader->use();
            auto model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(3, 1, -2));
            model = glm::scale(model, glm::vec3(0.2f));
            lightShader->setFloat3("lightColor", lightColor);
            lightShader->setMatrix4("model", model);
            lightShader->setMatrix4("view", oribitCamera->getViewMatrix());
            lightShader->setMatrix4("projection", projection);
            lightVAO->bind();
            glDrawArrays(GL_TRIANGLES, 0, 36);
            lightVAO->unbind();

            window->swapBuffer();
        }
    }
private:
    std::unique_ptr<Window> window;
    std::unique_ptr<Shader> containerShader;
    std::unique_ptr<Shader> lightShader;
    std::unique_ptr<VertexArray> containerVAO;
    std::unique_ptr<VertexArray> lightVAO;
    std::unique_ptr<OribitCamera> oribitCamera;

    bool dragging = false;
    double lastX, lastY;
    double curX, curY;

    void init() {
        float vertices[] = {
            -0.5f, -0.5f, -0.5f, 
             0.5f, -0.5f, -0.5f,  
             0.5f,  0.5f, -0.5f,  
             0.5f,  0.5f, -0.5f,  
            -0.5f,  0.5f, -0.5f, 
            -0.5f, -0.5f, -0.5f, 
    
            -0.5f, -0.5f,  0.5f, 
             0.5f, -0.5f,  0.5f,  
             0.5f,  0.5f,  0.5f,  
             0.5f,  0.5f,  0.5f,  
            -0.5f,  0.5f,  0.5f, 
            -0.5f, -0.5f,  0.5f, 
    
            -0.5f,  0.5f,  0.5f, 
            -0.5f,  0.5f, -0.5f, 
            -0.5f, -0.5f, -0.5f, 
            -0.5f, -0.5f, -0.5f, 
            -0.5f, -0.5f,  0.5f, 
            -0.5f,  0.5f,  0.5f, 
    
             0.5f,  0.5f,  0.5f,  
             0.5f,  0.5f, -0.5f,  
             0.5f, -0.5f, -0.5f,  
             0.5f, -0.5f, -0.5f,  
             0.5f, -0.5f,  0.5f,  
             0.5f,  0.5f,  0.5f,  
    
            -0.5f, -0.5f, -0.5f, 
             0.5f, -0.5f, -0.5f,  
             0.5f, -0.5f,  0.5f,  
             0.5f, -0.5f,  0.5f,  
            -0.5f, -0.5f,  0.5f, 
            -0.5f, -0.5f, -0.5f, 
    
            -0.5f,  0.5f, -0.5f, 
             0.5f,  0.5f, -0.5f,  
             0.5f,  0.5f,  0.5f,  
             0.5f,  0.5f,  0.5f,  
            -0.5f,  0.5f,  0.5f, 
            -0.5f,  0.5f, -0.5f, 
        };

        VertexBuffer containerVBO(GL_ARRAY_BUFFER);
        containerVBO.upload(vertices, sizeof(vertices) / sizeof(float));
        std::vector<VertexAttribute> containerAttributes = {
            VertexAttribute {
                0,
                3,
                AttributeType::Float,
                false,
                sizeof(float) * 3,
                (void*)0,
            }
        };

        containerVAO->bind();
        containerVAO->addVertexBuffer(containerVBO, containerAttributes);
        containerVAO->unbind();

        lightVAO->bind();
        lightVAO->addVertexBuffer(containerVBO, containerAttributes);
        lightVAO->unbind();
    }
};


int main() {
    ColorChapter app(800, 600, "2_1_1_Color");
    try {
        app.run();
    } catch(const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}