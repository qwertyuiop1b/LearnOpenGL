#include <exception>
#include <string>
#include <memory>
#include <iostream>
#include "utils/FPS.h"
#include "utils/Shader.h"
#include "utils/Texture.h"
#include "utils/VertexArray.h"
#include "utils/Window.h"


class Cube {
public:
    Cube(unsigned int width, unsigned int height, const std::string& title)
    : window(std::make_unique<Window>(width, height, title))
    , shader(std::make_unique<Shader>("shaders/01_shaders/1_5_2_Cube.vert", "shaders/01_shaders/1_5_2_Cube.frag"))
    , vao(std::make_unique<VertexArray>())
    , containerTexture(std::make_unique<Texture>("assets/textures/container.jpg"))
    , awesomeFaceTexture(std::make_unique<Texture>("assets/textures/awesomeface.png")) {
        init();
    }

    void run() {
        FPS fps(120);
        auto elapsedTime = 0.0f;
        while (!window->shouldClose()) {
            window->pollEvents();
            fps.update();
            elapsedTime += fps.getDeltaTime();

            glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            shader->use();
            containerTexture->bind(0);
            awesomeFaceTexture->bind(1);
            shader->setInt("uTexture", 0);
            shader->setInt("uTexture2", 1);
            auto model = glm::mat4(1.0f);
            model = glm::rotate(model, glm::radians(elapsedTime * 30.f), glm::vec3(0.5, 1.0f, 0.0f));
            auto view = glm::mat4(1.0f);
            view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            auto projection = glm::mat4(1.0f);
            projection = glm::perspective(glm::radians(45.0f), (float)window->getWidth() / (float)window->getHeight(), 0.1f, 100.0f);
            shader->setMatrix4("model", model);
            shader->setMatrix4("view", view);
            shader->setMatrix4("projection", projection); 
            vao->bind();
            glDrawArrays(GL_TRIANGLES, 0, 36);
            window->swapBuffer();
        }
    }
private:
    std::unique_ptr<Window> window;
    std::unique_ptr<Shader> shader;
    std::unique_ptr<VertexArray> vao;
    std::unique_ptr<Texture> containerTexture;
    std::unique_ptr<Texture> awesomeFaceTexture;

    void init() {
        float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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
                5 * sizeof(float),
                (void*)0,
            },
            VertexAttribute {
                1,
                2,
                AttributeType::Float,
                false,
                5 * sizeof(float),
                (void*)(3 * sizeof(float)),
            },
        };
        vao->addVertexBuffer(vbo, attrs);
        vao->unbind();
        vbo.unbind();

        glEnable(GL_DEPTH_TEST);
    }
};


int main() {
    try {
        Cube app(800, 600, "1_5_2_Cube");
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}