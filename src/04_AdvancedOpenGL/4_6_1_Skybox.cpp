#include "glm/ext/matrix_clip_space.hpp"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"
#include "utils/Application.h"
#include "utils/OribitCamera.h"
#include "utils/Shader.h"
#include "utils/VertexArray.h"
#include "utils/TextureCube.h"
#include "utils/VertexBuffer.h"
#include <string>
#include <vector>


class Skybox: public Application {
public:
    Skybox(unsigned int width, unsigned int height, const std::string &title): Application(width, height, title) {
        init();
    }


    virtual void render() override {
        const auto leftMouseBtnState = glfwGetMouseButton(getWindow(), GLFW_MOUSE_BUTTON_LEFT);
        if (leftMouseBtnState == GLFW_PRESS) {
            if (!dragging) {
                glfwGetCursorPos(getWindow(), &lastX, &lastY);
                dragging = true;
            }
            glfwGetCursorPos(getWindow(), &curX, &curY);
            const auto deltaX = curX - lastX;
            const auto deltaY = curY - lastY;
            oribitCamera.rotateAzimuth(glm::radians(static_cast<float>(deltaX) * 0.5f));
            oribitCamera.rotatePolar(glm::radians(static_cast<float>(deltaY) * 0.5f));
            lastX = curX;
            lastY = curY;
        } else {
            dragging = false;
        }

        glViewport(0, 0, getWidth(), getHeight());
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDepthMask(GL_FALSE); // Disable depth writing for skybox
        skyboxShader.use();
        skyboxTexture.bind();
        skyboxVao.bind();
        glm::mat4 view = glm::mat4(glm::mat3(oribitCamera.getViewMatrix()));
        glm::mat4 projection = glm::perspective(glm::radians(45.f), static_cast<float>(getWidth()) / static_cast<float>(getHeight()), 0.1f, 100.f);
        skyboxShader.setMatrix4("view", view);
        skyboxShader.setMatrix4("projection", projection);

        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthMask(GL_TRUE); // Re-enable depth writing
    }

    virtual void init(unsigned int glVersionMajor = 3, unsigned int glVersionMinor = 3) override {
        Application::init(glVersionMajor, glVersionMinor);

        lastX = getWidth() / 2.0;
        lastY = getHeight() / 2.0;

        // skybox vao
        float skyboxVertices[] = {
            // positions
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
        };

        skyboxVao.create();
        skyboxVao.bind();
        VertexBuffer skyboxVbo;
        skyboxVbo.upload(skyboxVertices, sizeof(skyboxVertices) / sizeof(float), BufferUsage::StaticDraw);
        std::vector<VertexAttribute> skyboxAttrs {
            VertexAttribute{0, 3, AttributeType::Float, GL_FALSE, 3 * sizeof(float), (void*)0},
        };
        skyboxVao.addVertexBuffer(skyboxVbo, skyboxAttrs);
        skyboxVao.unbind();
        skyboxVbo.unbind();

        if (skyboxTexture.loadFromFiles(skyboxFaces)) {
            std::cout << "Skybox texture loaded successfully. texture id: " << skyboxTexture.getId()  << std::endl;
        }

        skyboxShader.loadFromfile("shaders/04_shaders/4_6_1_skybox.vert", "shaders/04_shaders/4_6_1_skybox.frag");
        skyboxShader.use();
        skyboxShader.setInt("skyboxTexture", 0);

        glEnable(GL_DEPTH_TEST);
    }

private:
    // orbit camera control;
    bool dragging = false;
    double lastX, lastY;
    double curX, curY;
    OribitCamera oribitCamera{
        glm::vec3(0.0f, 0.0f, 0.0f),   // Center
        glm::vec3(0.0f, 1.0f, 0.0f), // Up vector
        6.0f,                                  // Radius
        1.0f,                               // Min radius
        glm::radians(0.f),      // Azimuth angle
        glm::radians(0.f)         // Polar angle
    };


    std::vector<std::string> skyboxFaces = {
        "textures/lake_skybox/right.jpg",
        "textures/lake_skybox/left.jpg",
        "textures/lake_skybox/top.jpg",
        "textures/lake_skybox/bottom.jpg",
        "textures/lake_skybox/front.jpg",
        "textures/lake_skybox/back.jpg",
    };
    // std::vector<std::string> skyboxFaces = {
    //     "textures/roblox_skybox/px.png",
    //     "textures/roblox_skybox/nx.png",
    //     "textures/roblox_skybox/py.png",
    //     "textures/roblox_skybox/ny.png",
    //     "textures/roblox_skybox/pz.png",
    //     "textures/roblox_skybox/nz.png",
    // };

    TextureCube skyboxTexture;
    Shader skyboxShader;
    VertexArray skyboxVao;


};


int main() {
    Skybox app(800, 600, "Skybox Example");
    app.run();
    return 0;
}