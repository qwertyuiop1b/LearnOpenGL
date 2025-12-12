#include "utils/Application.h"
#include "utils/Shader.h"
#include "utils/VertexArray.h"
#include "utils/VertexBuffer.h"
#include "utils/Window.h"
#include <memory>
#include <vector>

class TwoShaderTriangle {
public:
    TwoShaderTriangle(unsigned int width, unsigned int height, const std::string& title)
        : window(std::make_unique<Window>(width, height, title)) 
        , twoShaderTriangleShader(std::make_unique<Shader>("shaders/01_shaders/1_1_5_TwoShaderTriangle.vert", "shaders/01_shaders/1_1_5_TwoShaderTriangle.frag"))
        , twoShaderTriangleShader2(std::make_unique<Shader>("shaders/01_shaders/1_1_5_TwoShaderTriangle.vert", "shaders/01_shaders/1_1_5_2_TwoShaderTriangle.frag"))
        , vao(std::make_unique<VertexArray>())
        , vao2(std::make_unique<VertexArray>()) {
        init();
    }

    void run() {
        while (!window->shouldClose()) {
            window->pollEvents();
            glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
    
            twoShaderTriangleShader->use();
            vao->bind();
            glDrawArrays(GL_TRIANGLES, 0, 3);
    
            twoShaderTriangleShader2->use();
            vao2->bind();
            glDrawArrays(GL_TRIANGLES, 0, 3);
            window->swapBuffer();
        }
    }

private:
    std::unique_ptr<Window> window;
    std::unique_ptr<Shader> twoShaderTriangleShader;
    std::unique_ptr<Shader> twoShaderTriangleShader2;
    std::unique_ptr<VertexArray> vao;
    std::unique_ptr<VertexArray> vao2;

    void init() {
        // create vao vbo ebo
        float vertices[] {
            -0.5f, 0.5f, 0.f,
            0.f, 0.f, 0.f,
            -0.5f, -0.5f, 0.f,
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

        // vao2
        vao2->bind();
        float vertices2[] {
            0.f, 0.f, 0.f,
            0.5f, 0.5f, 0.f,
            0.5f, -0.5f, 0.f,
        };
        VertexBuffer vbo2(GL_ARRAY_BUFFER);
        vbo2.upload<float>(vertices2, sizeof(vertices2) / sizeof(float));
        std::vector<VertexAttribute> attrs2 {
            VertexAttribute{
                0,
                3,
                AttributeType::Float,
                false,
                0,
                0,
            }
        };
        vao2->addVertexBuffer(vbo2, attrs2);
        vbo2.unbind();
        vao2->unbind();
    }
};

int main() {
    TwoShaderTriangle app(800, 600, "1_1_5_TwoShaderTriangle");
    app.run();
    return 0;
}