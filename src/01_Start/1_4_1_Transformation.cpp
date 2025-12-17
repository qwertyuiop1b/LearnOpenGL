#include "utils/FPS.h"
#include "utils/Shader.h"
#include "utils/Texture.h"
#include "utils/VertexArray.h"
#include "utils/Window.h"
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <math.h>
#include <string>
#include <memory>



class Transformation {
public:
    Transformation(unsigned int width, unsigned int height, const std::string& title) 
    : window(std::make_unique<Window>(width, height, title))
    , shader(std::make_unique<Shader>("shaders/01_shaders/1_4_1_Transformation.vert", "shaders/01_shaders/1_4_1_Transformation.frag"))
    , vao(std::make_unique<VertexArray>())
    , containerTexture(std::make_unique<Texture>("assets/textures/container.jpg"))
    , awesomeFaceTexture(std::make_unique<Texture>("assets/textures/awesomeface.png")) {
        init();

    }

    void run() {
        FPS fps(120);
        auto elapsedTime = 0.0f;
        auto rotationPerSecond = 3.0f;
        auto rotationAxis = glm::vec3(0.0f, 0.0f, 1.0f);
        while (!window->shouldClose()) {
            fps.update();
            window->pollEvents();
            glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            shader->use();
            containerTexture->bind(0);
            awesomeFaceTexture->bind(1);
            shader->setInt("uTexture", 0);
            shader->setInt("uTexture2", 1);

            elapsedTime += fps.getDeltaTime();
            model = glm::mat4(1.0f);
            auto angle = rotationPerSecond * elapsedTime;
            auto c = cos(angle);
            auto s = sin(angle);
            // T -> R  or R -> T
            model = glm::rotate(model, glm::radians(angle * 20), rotationAxis);
            model = glm::translate(model, glm::vec3(0.5, 0.5, 0.0f));
            shader->setMatrix4("model", model);
            vao->bind();

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            window->swapBuffer();
        }
    }
private:
    std::unique_ptr<Window> window;
    std::unique_ptr<Shader> shader;
    std::unique_ptr<VertexArray> vao;
    std::unique_ptr<Texture> containerTexture;
    std::unique_ptr<Texture> awesomeFaceTexture;

    glm::mat4 model = glm::mat4(1.0f);

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
    }
};



int main() {
    try {
        Transformation app(800, 600, "1_4_1_Transformation");
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}