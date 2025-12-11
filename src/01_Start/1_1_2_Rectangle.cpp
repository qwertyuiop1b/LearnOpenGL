#include "utils/Shader.h"
#include "utils/VertexArray.h"
#include "utils/VertexBuffer.h"
#include "utils/Window.h"
#include <memory>


class Rectangle2App {
public:
    Rectangle2App(unsigned int width, unsigned int height, const std::string& title)
        : window(new Window(width, height, title))
        , shader(new Shader("shaders/01_shaders/1_1_2_Rectangle.vert", "shaders/01_shaders/1_1_2_Rectangle.frag"))
        , vbo(new VertexBuffer(GL_ARRAY_BUFFER))
        , vao(new VertexArray())
        , ibo(new VertexBuffer(GL_ELEMENT_ARRAY_BUFFER))
        {
            init();
    }

    void init() {
        float vertices[] = {
            // position, color
            0.5, 0.5, 0, 1.0, 0.0, 0.0,   // top right
            -0.5, 0.5, 0, 1.0, 1.0, 1.0,  // top left
            -0.5, -0.5, 0, 0.0, 0.0, 1.0, // bottom left
            0.5, -0.5, 0, 0.0, 1.0, 0.0,  // bottom right
        };

        unsigned int indices[] = {
            0, 1, 2,
            0, 2, 3,
        };

        vao->bind();
        vbo->bind();
        vbo->upload(vertices, sizeof(vertices));

        // attributes
        std::vector<VertexAttribute> attributes {
            VertexAttribute{
                0,
                3,
                AttributeType::Float,
                false,
                6 * sizeof(AttributeType::Float),
                0,
            },
            VertexAttribute{
                1,
                3,
                AttributeType::Float,
                false,
                6 * sizeof(AttributeType::Float),
                (void*)( 3 * sizeof(AttributeType::Float)),
            },
        };
        vao->addVertexBuffer(*vbo, attributes);
        
        ibo->bind();
        ibo->upload(indices, sizeof(indices));
        vao->setIndexBuffer(*ibo);
        vao->unbind();

        // cullface
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
    }

    void run() {
        while (!window->shouldClose()) {
            window->pollEvents();

            shader->use();
            glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            vao->bind();
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
            window->swapBuffer();
        }
    }
private:
    std::unique_ptr<Window> window;
    std::unique_ptr<Shader> shader;
    std::unique_ptr<VertexArray> vao;
    std::unique_ptr<VertexBuffer> vbo;
    std::unique_ptr<VertexBuffer> ibo;
};


int main() {
    Rectangle2App app(800, 600, "1_1_2_Rectangle2");
    app.run();
    return 0;
}