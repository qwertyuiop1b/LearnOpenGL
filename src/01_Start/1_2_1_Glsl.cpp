#include <memory>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "utils/Shader.h"
#include "utils/VertexArray.h"
#include "utils/VertexBuffer.h"
#include "utils/Window.h"


class Glsl {
public:
    Glsl(unsigned int width, unsigned int height, const std::string& title)
    : window(std::make_unique<Window>(width, height, title))
    , shader(std::make_unique<Shader>("shaders/01_shaders/1_2_1_Glsl.vert", "shaders/01_shaders/1_2_1_Glsl.frag"))
    , vao(std::make_unique<VertexArray>()) {
        init();
    }

    void run() {
        while(!window->shouldClose()) {
            window->pollEvents();
            
            glClearColor(0.3, 0.1, 0.5, 0.8);
            glClear(GL_COLOR_BUFFER_BIT);
            shader->use();
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
    Glsl app(800, 600, "1_2_1_Glsl");
    app.run();
    return 0;
}