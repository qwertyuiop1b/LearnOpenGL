#include <cmath>
#include <math.h>
#include <memory>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "utils/Shader.h"
#include "utils/VertexArray.h"
#include "utils/VertexBuffer.h"
#include "utils/Window.h"


class GlslUniform {
public:
    GlslUniform(unsigned int width, unsigned int height, const std::string& title)
    : window(std::make_unique<Window>(width, height, title))
    , shader(std::make_unique<Shader>("shaders/01_shaders/1_2_2_GlslUniform.vert", "shaders/01_shaders/1_2_2_GlslUniform.frag"))
    , vao(std::make_unique<VertexArray>()) {
        init();
    }

    void run() {
        while(!window->shouldClose()) {
            window->pollEvents();
            auto time = static_cast<float>(glfwGetTime());
            auto timev = sin(time);

            glClearColor(0.3, 0.1, 0.5, 0.8);
            glClear(GL_COLOR_BUFFER_BIT);
            shader->use();
            shader->setFloat4("uVertexColor", 1.0, 0.5, std::abs(timev), 1.0);
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
    GlslUniform app(800, 600, "1_2_2_GlslUniform");
    app.run();
    return 0;
}