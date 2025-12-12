#include <memory>
#include <vector>
#include "utils/Window.h"
#include "utils/Application.h"
#include "utils/Shader.h"
#include "utils/VertexArray.h"
#include "utils/VertexBuffer.h"

class TwoTriangle {
public:
    TwoTriangle(unsigned int width, unsigned int height, const std::string& title)
        : window(std::make_unique<Window>(width, height, title))
        , twoTriangleShader(std::make_unique<Shader>("shaders/01_shaders/1_1_3_TwoTriangle.vert", "shaders/01_shaders/1_1_3_TwoTriangle.frag"))
        , vao(std::make_unique<VertexArray>()) {
        init();
    }
    
    void run() {
        while (!window->shouldClose()) {
            window->pollEvents();
            glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
    
            twoTriangleShader->use();
            vao->bind();
            glDrawArrays(GL_TRIANGLES, 0, 6);
            window->swapBuffer();
        }
    }

private:
    std::unique_ptr<Window> window;
    std::unique_ptr<Shader> twoTriangleShader;
    std::unique_ptr<VertexArray> vao;

    void init() {
        float vertices[] {
            -0.5f, 0.5f, 0.f,
            0.f, 0.f, 0.f,
            -0.5f, -0.5f, 0.f,
            0.f, 0.f, 0.f,
            0.5f, 0.5f, 0.f,
            0.5f, -0.5f, 0.f
        };

        vao->bind();
        VertexBuffer vbo(GL_ARRAY_BUFFER);
        vbo.upload<float>(vertices, sizeof(vertices) / sizeof(float));
        // attributes
        std::vector<VertexAttribute> attrs {
            VertexAttribute{
                0,
                3,
                AttributeType::Float,
                false,
                0,
                0,
            }
        };
        vao->addVertexBuffer(vbo, attrs);
        vbo.unbind();
        vao->unbind();
    }
};

int main() {
    TwoTriangle app(800, 600, "1_1_3_TwoTriangle");
    app.run();
    return 0;
}