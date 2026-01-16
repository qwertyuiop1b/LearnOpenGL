#include "utils/OribitCamera.h"
#include "utils/Window.h"
#include "utils/Shader.h"
#include "utils/Texture.h"
#include "utils/VertexArray.h"
#include "utils/VertexBuffer.h"
#include "utils/Input.h"
#include <memory>
#include <string>
#include <vector>


class SpecularRevert {
public:
    SpecularRevert(unsigned int width, unsigned int height, const std::string& title)
    : window(std::make_unique<Window>(width, height, title))
    , diffuseTex(std::make_unique<Texture>("assets/textures/container2.png"))
    , specularTex(std::make_unique<Texture>("assets/textures/container2_specular.png"))
    , cubeShader(std::make_unique<Shader>("shaders/02_shaders/2_4_4_SpecularRevert.vert", "shaders/02_shaders/2_4_4_SpecularRevert.frag"))
    , lightShader(std::make_unique<Shader>("shaders/02_shaders/2_1_1_Light.vert", "shaders/02_shaders/2_1_1_Light.frag"))
    , cubeVao(std::make_unique<VertexArray>())
    , lightVao(std::make_unique<VertexArray>())
    , orbitCamera(std::make_unique<OribitCamera>(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f), 10.f, 1.f, glm::radians(90.f), glm::radians(45.f)))
    {
        init();
    }

    void run() {
        glEnable(GL_DEPTH_TEST);
        glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
        glm::vec3 lightPos(3, 0, 5);

        Input& input = Input::getInstance();

        lastX = window->getWidth() / 2.0;
        lastY = window->getHeight() / 2.0;

        while (!window->shouldClose()) {
            window->pollEvents();

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
                orbitCamera->rotateAzimuth(glm::radians(static_cast<float>(deltaX) * 0.5f));
                orbitCamera->rotatePolar(glm::radians(static_cast<float>(deltaY) * 0.5f));
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

            // cube
            glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(window->getWidth()) / static_cast<float>(window->getHeight()), 0.1f, 100.0f);
            cubeShader->use();

            cubeShader->setFloat3("viewPos", orbitCamera->getEye());
            cubeShader->setMatrix4("model", glm::mat4(1.0f));
            cubeShader->setMatrix4("view", orbitCamera->getViewMatrix());
            cubeShader->setMatrix4("projection", projection);

            cubeShader->setInt("material.diffuse", 0);
            cubeShader->setInt("material.specular", 1);
            cubeShader->setFloat("material.shininess", 32.0f);

            cubeShader->setFloat3("light.position", lightPos);
            cubeShader->setFloat3("light.ambient", glm::vec3(0.3f, 0.3f, 0.3f));
            cubeShader->setFloat3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
            cubeShader->setFloat3("light.specular", glm::vec3(0.5, 0.5, 0.5));

            cubeVao->bind();
            diffuseTex->bind(0);
            specularTex->bind(1);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            cubeVao->unbind();

            // light
            lightShader->use();
            auto model = glm::mat4(1.0);
            model = glm::translate(model, lightPos);
            model = glm::scale(model, glm::vec3(0.2f));
            lightShader->setFloat3("lightColor", lightColor);
            lightShader->setMatrix4("model", model);
            lightShader->setMatrix4("view", orbitCamera->getViewMatrix());
            lightShader->setMatrix4("projection", projection);
            lightVao->bind();
            glDrawArrays(GL_TRIANGLES, 0, 36);
            lightVao->unbind();

            window->swapBuffer();
        }
    }
private:
    std::unique_ptr<Window> window;
    std::unique_ptr<Texture> diffuseTex;
    std::unique_ptr<Texture> specularTex;
    std::unique_ptr<Shader> cubeShader;
    std::unique_ptr<Shader> lightShader;
    std::unique_ptr<VertexArray> cubeVao;
    std::unique_ptr<VertexArray> lightVao;
    std::unique_ptr<OribitCamera> orbitCamera;

    bool dragging = false;
    double lastX, lastY;
    double curX, curY;

    void init() {
        float vertices[] = {
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
        };

        VertexBuffer vbo;
        vbo.upload(vertices, sizeof(vertices));
        std::vector<VertexAttribute> attrs {
            VertexAttribute{0, 3, AttributeType::Float, false, 8 * sizeof(float), (void*)0},
            VertexAttribute{1, 3, AttributeType::Float, false, 8 * sizeof(float), (void*)(3 * sizeof(float))},
            VertexAttribute{2, 2, AttributeType::Float, false, 8 * sizeof(float), (void*)(6 * sizeof(float))},
        };
        cubeVao->bind();
        cubeVao->addVertexBuffer(vbo, attrs);
        cubeVao->unbind();

        lightVao->bind();
        lightVao->addVertexBuffer(vbo, {attrs[0]});
        lightVao->unbind();
    }
};


int main() {
    SpecularRevert app(800, 600, "2.4.4.SpecularRevert");
    try {
        app.run();
    } catch(const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return 0;
}