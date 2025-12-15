#include <algorithm>
#include <memory>
#include <string>
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "utils/VertexArray.h"
#include "utils/VertexBuffer.h"
#include "utils/Shader.h"
#include "utils/Window.h"

// 0 ~ 1 ~ 0
// @params value [0, 1]
// @return [0,1]
float pingpong(float value) {
    return std::min(2 * value, -2 * value + 2);
}


class GlslVertexUniform {
public:
    GlslVertexUniform(unsigned int width, unsigned int height, const std::string& title)
    : window(std::make_unique<Window>(width, height, title))
    , shader(std::make_unique<Shader>("shaders/01_shaders/1_2_3_GlslVertexUniform.vert", "shaders/01_shaders/1_2_3_GlslVertexUniform.frag"))
    , vao(std::make_unique<VertexArray>()) {
        init();
    }

    void run() {
        auto lastTime = glfwGetTime();
        while(!window->shouldClose()) {
            window->pollEvents();
            glClearColor(0.5, 0.3, 0.2, 1.0);
            glClear(GL_COLOR_BUFFER_BIT);
            auto currentTime = glfwGetTime();
            auto elapsedTime = currentTime - lastTime;
            if (elapsedTime >= 5.0) {
                elapsedTime = 0;
                lastTime = currentTime;
            }
            // range -0.2 ~ 0.2
            auto v = pingpong(elapsedTime * 0.2) * 0.4 - 0.2;
            shader->use();
            shader->setFloat2("uOffset", v, 0.0);
            vao->bind();
            glDrawArrays(GL_TRIANGLES, 0, 3);
            window->swapBuffer();
        }
    }
private:
    std::unique_ptr<Window> window;
    std::unique_ptr<Shader> shader;
    std::unique_ptr<VertexArray> vao;

    void init() {
        float vertices[] = {
            -0.5, -0.5, 0.0,
            0.0, 0.5, 0.0,
            0.5, -0.5, 0.0
        };

        vao->bind();
        VertexBuffer vbo;
        vbo.upload(vertices, sizeof(vertices) / sizeof(float));
        std::vector<VertexAttribute> attrs {
            VertexAttribute {
                0,
                3,
                AttributeType::Float,
                false,
                3 * sizeof(float),
                (void*)0
            }
        };
        vao->addVertexBuffer(vbo, attrs);
        vao->unbind();
        vbo.unbind();
    }
};


int main() {
    GlslVertexUniform app(800, 600, "1_2_3_GlslVertexUniform");
    app.run();
    return 0;
}