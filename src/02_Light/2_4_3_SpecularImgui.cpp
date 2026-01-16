#include "utils/Window.h"
#include "utils/OribitCamera.h"
#include "utils/Shader.h"
#include "utils/Texture.h"
#include "utils/VertexArray.h"
#include "utils/VertexBuffer.h"
#include "utils/Input.h"
#include "utils/ImGuiManager.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <memory>
#include <string>
#include <vector>


class SpecularImgui {
public:
    SpecularImgui(unsigned int width, unsigned int height, const std::string& title)
    : window(std::make_unique<Window>(width, height, title))
    , diffuseTex(std::make_unique<Texture>("assets/textures/container2.png"))
    , specularTex(std::make_unique<Texture>("assets/textures/container2_specular.png"))
    , cubeShader(std::make_unique<Shader>("shaders/02_shaders/2_4_2_SpecularMap.vert", "shaders/02_shaders/2_4_2_SpecularMap.frag"))
    , lightShader(std::make_unique<Shader>("shaders/02_shaders/2_1_1_Light.vert", "shaders/02_shaders/2_1_1_Light.frag"))
    , cubeVao(std::make_unique<VertexArray>())
    , lightVao(std::make_unique<VertexArray>())
    , orbitCamera(std::make_unique<OribitCamera>(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f), 10.f, 1.f, glm::radians(90.f), glm::radians(45.f)))
    , imguiManager(std::make_unique<ImGuiManager>(window->getGLFWWindow()))
    {
        init();
    }

    void run() {
        glEnable(GL_DEPTH_TEST);
        
        // 光照属性
        glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
        glm::vec3 lightPos(3, 0, 5);
        glm::vec3 lightAmbient(0.2f, 0.2f, 0.2f);
        glm::vec3 lightDiffuse(0.5f, 0.5f, 0.5f);
        glm::vec3 lightSpecular(1.0f, 1.0f, 1.0f);
        float shininess = 32.0f;

        Input& input = Input::getInstance();

        lastX = window->getWidth() / 2.0;
        lastY = window->getHeight() / 2.0;

        while (!window->shouldClose()) {
            window->pollEvents();

            // 检查ImGui是否捕获了鼠标或键盘输入
            bool ImGuiCaptured = ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard;

            // 只有当ImGui没有捕获输入时，才允许OrbitCamera操作
            if (!ImGuiCaptured && input.GetMouseButton(GLFW_MOUSE_BUTTON_LEFT)) {
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
            cubeShader->setFloat("material.shininess", shininess);

            cubeShader->setFloat3("light.position", lightPos);
            cubeShader->setFloat3("light.ambient", lightAmbient);
            cubeShader->setFloat3("light.diffuse", lightDiffuse);
            cubeShader->setFloat3("light.specular", lightSpecular);

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

            // ImGui渲染
            imguiManager->newFrame();
            
            // 创建光照控制窗口
            ImGui::Begin("Lighting Controls");
            
            // 光源位置控制
            ImGui::Text("Light Position");
            ImGui::SliderFloat("X", &lightPos.x, -5.0f, 5.0f);
            ImGui::SliderFloat("Y", &lightPos.y, -5.0f, 5.0f);
            ImGui::SliderFloat("Z", &lightPos.z, -5.0f, 5.0f);
            
            // 光源颜色控制
            ImGui::Text("Light Color");
            ImGui::ColorEdit3("Color", (float*)&lightColor);
            
            // 环境光控制
            ImGui::Text("Ambient");
            ImGui::ColorEdit3("Ambient", (float*)&lightAmbient);
            
            // 漫反射光控制
            ImGui::Text("Diffuse");
            ImGui::ColorEdit3("Diffuse", (float*)&lightDiffuse);
            
            // 镜面反射光控制
            ImGui::Text("Specular");
            ImGui::ColorEdit3("Specular", (float*)&lightSpecular);
            
            // 反光度控制
            ImGui::Text("Shininess");
            ImGui::SliderFloat("Shininess", &shininess, 1.0f, 128.0f);
            
            ImGui::End();
            
            imguiManager->render();

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
    std::unique_ptr<ImGuiManager> imguiManager;

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
    SpecularImgui app(800, 600, "2.4.3.SpecularImgui");
    try {
        app.run();
    } catch(const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return 0;
}
