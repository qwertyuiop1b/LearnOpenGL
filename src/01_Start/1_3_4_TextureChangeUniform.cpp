#include <memory>
#include "utils/FPS.h"
#include "utils/Window.h"
#include "utils/Shader.h"
#include "utils/Texture.h"
#include "utils/VertexArray.h"
#include "utils/VertexBuffer.h"

class TextureChangeUniform {
public:
TextureChangeUniform(unsigned int width, unsigned int height, const std::string& title)
        : window(std::make_unique<Window>(width, height, title))
        , shader(std::make_unique<Shader>("shaders/01_shaders/1_3_4_TextureChangeUniform.vert", "shaders/01_shaders/1_3_4_TextureChangeUniform.frag"))
        , vao(std::make_unique<VertexArray>())
        , texture1(std::make_unique<Texture>("assets/textures/container.jpg"))
        , texture2(std::make_unique<Texture>("assets/textures/awesomeface.png")) {
        init();
    }

    void run() {
        FPS fps(60);
        while (!window->shouldClose()) {
            fps.update();
            window->pollEvents();
            auto deltaTime = fps.getDeltaTime();
            handleMixValue(deltaTime);

            glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            shader->use();
            texture1->bind(0);
            texture2->bind(1);
            shader->setInt("uTexture", 0);
            shader->setInt("uTexture2", 1);
            shader->setFloat("uMixValue", mixValue);
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

    float mixValue = 0.5f;
    void handleMixValue(float deltaTime) {
        if (glfwGetKey(window->getGLFWWindow(), GLFW_KEY_UP) == GLFW_PRESS) {
            mixValue += 0.2f * deltaTime;
        }
        if (glfwGetKey(window->getGLFWWindow(), GLFW_KEY_DOWN) == GLFW_PRESS) {
            mixValue -= 0.2f * deltaTime;
        }
        if (mixValue < 0.0f) {
            mixValue = 0.0f;
        }
        if (mixValue > 1.0f) {
            mixValue = 1.0f;
        }
    }


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
        TextureChangeUniform app(800, 600, "1_3_4_TextureChangeUniform");
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}