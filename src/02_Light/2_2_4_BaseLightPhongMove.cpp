#include <memory>
#include <string>
#include "glm/ext/matrix_transform.hpp"
#include "utils/FPS.h"
#include "utils/OribitCamera.h"
#include "utils/Shader.h"
#include "utils/VertexArray.h"
#include "utils/VertexBuffer.h"
#include "utils/Window.h"
#include "utils/Input.h"

class BaseLightSpecular {
public:
    BaseLightSpecular(unsigned int width, unsigned int height, const std::string& title)
    : window(std::make_unique<Window>(width, height, title))
    , lightShader(std::make_unique<Shader>("shaders/02_shaders/2_1_1_Light.vert", "shaders/02_shaders/2_1_1_Light.frag"))
    , cubeShader(std::make_unique<Shader>("shaders/02_shaders/2_2_3_BaseLightSpecular.vert", "shaders/02_shaders/2_2_3_BaseLightSpecular.frag"))
    , lightVao(std::make_unique<VertexArray>())
    , cubeVao(std::make_unique<VertexArray>())
    , oribitCamera(std::make_unique<OribitCamera>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 10.0f, 0.1f, 0.0f, 0.0f)) {
        init();

    } 


    void run() {
        glEnable(GL_DEPTH_TEST);
        glm::vec3 objectColor(1.0f, 0.5f, 0.31f);
        glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
        glm::vec3 lightPos(3, 4, -2);
        Input& input = Input::getInstance();

        lastX = window->getWidth() / 2.0;
        lastY = window->getHeight() / 2.0;

        FPS fps(120);
        float elaspedTime = 0.0;

        while (!window->shouldClose()) {
            window->pollEvents();
            fps.update();
            elaspedTime += fps.getDeltaTime();
            if (input.GetMouseButton(GLFW_MOUSE_BUTTON_LEFT)) {
                if (!dragging) {
                    auto mousePos = input.GetMousePosition();
                    lastX = mousePos.x;
                    lastY = mousePos.y;
                    dragging = true;
                }
                auto mousePos = input.GetMousePosition();
                curX = mousePos.x;
                curY = mousePos.y;
                const auto deltaX = curX - lastX;
                const auto deltaY = curY - lastY;
                oribitCamera->rotateAzimuth(glm::radians(static_cast<float>(deltaX) * 0.5f));
                oribitCamera->rotatePolar(glm::radians(static_cast<float>(deltaY) * 0.5f));
                lastX = curX;
                lastY = curY;
            } else {
                dragging = false;
            }

            if (input.GetKey(GLFW_KEY_ESCAPE)) {
                window->exit();
            }

            glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(window->getWidth()) / static_cast<float>(window->getHeight()), 0.1f, 100.0f);
            
            // 计算light的变换矩阵（用于获取旋转后的位置）
            auto lightModel = glm::mat4(1.0);
            lightModel = glm::rotate(lightModel, elaspedTime, glm::vec3(0, 0, 1));
            lightModel = glm::translate(lightModel, lightPos);
            // 从变换矩阵中提取旋转后的世界坐标位置（第4列的前3个元素）
            glm::vec3 currentLightPos = glm::vec3(lightModel[3]);
            
            cubeShader->use();
            cubeShader->setFloat3("lightColor", lightColor);
            cubeShader->setFloat3("objectColor", objectColor);
            cubeShader->setFloat3("lightPos", currentLightPos);  // 使用旋转后的位置
            cubeShader->setFloat3("viewPos", oribitCamera->getEye());
            cubeShader->setMatrix4("model", glm::mat4(1.0f));
            cubeShader->setMatrix4("view", oribitCamera->getViewMatrix());
            cubeShader->setMatrix4("projection", projection);
            
            cubeVao->bind();
            glDrawArrays(GL_TRIANGLES, 0, 36);
            cubeVao->unbind();

            lightShader->use();
            auto model = lightModel;  // 复用已计算的变换矩阵
            model = glm::scale(model, glm::vec3(0.2f));  // 只添加缩放

            lightShader->setFloat3("lightColor", lightColor);
            lightShader->setMatrix4("model", model);
            lightShader->setMatrix4("view", oribitCamera->getViewMatrix());
            lightShader->setMatrix4("projection", projection);
            lightVao->bind();
            glDrawArrays(GL_TRIANGLES, 0, 36);
            lightVao->unbind();

            window->swapBuffer();
        }
    }


private:
    std::unique_ptr<Window> window;
    std::unique_ptr<Shader> lightShader;
    std::unique_ptr<Shader> cubeShader;
    std::unique_ptr<VertexArray> lightVao;
    std::unique_ptr<VertexArray> cubeVao;
    std::unique_ptr<OribitCamera> oribitCamera;

    bool dragging = false;
    double lastX, lastY;
    double curX, curY;

    void init() {
        float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
        };

        VertexBuffer vbo(GL_ARRAY_BUFFER);
        vbo.upload(vertices, sizeof(vertices) / sizeof(float));
        std::vector<VertexAttribute> attrs {
            VertexAttribute {
                0,
                3,
                AttributeType::Float,
                false,
                sizeof(float) * 6,
                (void*)0,
            },
            VertexAttribute {
                1,
                3,
                AttributeType::Float,
                false,
                sizeof(float) * 6,
                (void*)(sizeof(float) * 3),
            }
        };

        cubeVao->bind();
        cubeVao->addVertexBuffer(vbo, attrs);
        cubeVao->unbind();

        lightVao->bind();
        lightVao->addVertexBuffer(vbo, attrs);
        lightVao->unbind();
    }

};


int main() {
    BaseLightSpecular app(800, 600, "2_2_3_BaseLightSpecular");
    try {
        app.run();
    } catch(const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}