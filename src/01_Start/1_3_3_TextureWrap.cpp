#include <memory>
#include "utils/Window.h"
#include "utils/Shader.h"
#include "utils/Texture.h"
#include "utils/VertexArray.h"
#include "utils/VertexBuffer.h"

class TextureWrap {
public:
TextureWrap(unsigned int width, unsigned int height, const std::string& title)
        : window(std::make_unique<Window>(width, height, title))
        , shader(std::make_unique<Shader>("shaders/01_shaders/1_3_3_TextureWrap.vert", "shaders/01_shaders/1_3_3_TextureWrap.frag"))
        , vao(std::make_unique<VertexArray>())
        , texture1(std::make_unique<Texture>("textures/container.jpg"))
        , texture2(std::make_unique<Texture>("textures/awesomeface.png")) {
        init();
    }

    void run() {
        while (!window->shouldClose()) {
            window->pollEvents();
            glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            shader->use();
            texture1->bind(0);
            texture2->bind(1);
            shader->setInt("uTexture", 0);
            shader->setInt("uTexture2", 1);
            vao->bind();
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            window->swapBuffer();
        }
    }

private:
    std::unique_ptr<Window> window;
    std::unique_ptr<Shader> shader;
    std::unique_ptr<VertexArray> vao;
    std::unique_ptr<Texture> texture1;
    std::unique_ptr<Texture> texture2;

    void init() {
        float vertices[] = {
            // positions          // colors           // texture coords
             0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
             0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
        };

        unsigned int indices[] = {
            3, 2, 1,
            3, 1, 0,
        };

        vao->bind();
        VertexBuffer vbo(GL_ARRAY_BUFFER);
        vbo.upload(vertices, sizeof(vertices) / sizeof(float));
        std::vector<VertexAttribute> attrs {
            VertexAttribute {
                0,
                3,
                AttributeType::Float,
                false,
                8 * sizeof(float),
                (void*)0,
            },
            VertexAttribute {
                1,
                3,
                AttributeType::Float,
                false,
                8 * sizeof(float),
                (void*)(3 * sizeof(float)),
            },
            VertexAttribute {
                2,
                2,
                AttributeType::Float,
                false,
                8 * sizeof(float),
                (void*)(6 * sizeof(float)),
            },
        };
        vao->addVertexBuffer(vbo, attrs);
        VertexBuffer ebo(GL_ELEMENT_ARRAY_BUFFER);
        ebo.upload(indices, sizeof(indices) / sizeof(unsigned int));
        vao->setIndexBuffer(ebo);
        vao->unbind();
        vbo.unbind();
        ebo.unbind();

        texture2->setParameter(GL_TEXTURE_WRAP_S, (GLuint)GL_MIRRORED_REPEAT);
        texture2->setParameter(GL_TEXTURE_WRAP_T, (GLuint)GL_MIRRORED_REPEAT);
    }
};



int main() {
    try {
        TextureWrap app(800, 600, "1_3_3_TextureWrap");
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}