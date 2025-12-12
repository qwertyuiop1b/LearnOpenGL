#include "utils/Shader.h"
#include "utils/VertexArray.h"
#include "utils/Window.h"
#include <memory>
#include <string>


class AlphabetF {
public:
    AlphabetF(unsigned int width, unsigned int height, const std::string& title)
    : window(std::make_unique<Window>(width, height, title))
    , shader(std::make_unique<Shader>("shaders/01_shaders/1_1_6_AlphabetF.vert", "shaders/01_shaders/1_1_6_AlphabetF.frag"))
    , vao(std::make_unique<VertexArray>()) {
        init();
    }

    void run() {
        while (!window->shouldClose()) {
            window->pollEvents();
            glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            shader->use();
            vao->bind();
            glDrawArrays(GL_LINE_LOOP, 0, 10);
            window->swapBuffer();
        }
    }

    
private:
    std::unique_ptr<Window> window;
    std::unique_ptr<Shader> shader;
    std::unique_ptr<VertexArray> vao;


    void init() {
        float vertices[] = {
            -0.1, 0.6, 0.0,
            -0.1, -0.6, 0.0,
            0.1, -0.6, 0.0,
            0.1, -0.2, 0.0,
            0.5, -0.2, 0.0,
            0.5, 0.0, 0.0,
            0.1, 0.0, 0.0,
            0.1, 0.4, 0.0,
            0.5, 0.4, 0.0,
            0.5, 0.6, 0.0,
        };

        vao->bind();
        VertexBuffer vbo(GL_ARRAY_BUFFER);
        vbo.upload(vertices, sizeof(vertices) / sizeof(float));
        std::vector<VertexAttribute> attrs {
            VertexAttribute{
                0,
                3,
                AttributeType::Float,
                false,
                3 * sizeof(float),
                (void*)0,
            }
        };
        vao->addVertexBuffer(vbo, attrs);
        vbo.unbind();
        vao->unbind();
    }
};

int main() {
    AlphabetF app(800, 600, "1_1_6_AlphabetF");
    app.run();
    return 0;
}