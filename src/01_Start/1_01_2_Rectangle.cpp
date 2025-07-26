#include "utils/Application.h"
#include "utils/Shader.h"
#include "utils/VertexArray.h"
#include "utils/VertexBuffer.h"
#include <vector>

class RectangleApp: public Application {
public:
    RectangleApp(unsigned int width, unsigned int height, const std::string& title)
        : Application(width, height, title) {
        init();
        // beforeRender();
    }


    virtual void render() override {
        glClearColor(0.2f, 0.3f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // rectShader.use();
        // vao.bind();
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

private:
    // Shader rectShader{"shaders/01_shaders/01_01_Rectangle.vert", "shaders/01_shaders/01_01_Rectangle.frag"};

    VertexArray vao;

    // void beforeRender() {
    //     // create vao vbo ebo
    //     float vertices[] {
    //         // top left
    //         -0.5f, 0.5f, 0.f, 1.0f, 0.f, 0.f,
    //         // top right
    //         0.5f, 0.5f, 0.f, 0.f, 1.0f, 0.f,
    //         // bottom right
    //         0.5f, -0.5f, 0.f, 0.f, 0.f, 1.f,
    //         // bottom left
    //         -0.5f, -0.5f, 0.f, 1.f, 1.f, 1.f,
    //     };

    //     unsigned int indices[] {
    //         0, 1, 2,
    //         0, 2, 3,
    //     };

    //     VertexBuffer vbo(GL_ARRAY_BUFFER);
    //     vbo.upload<float>(vertices, sizeof(vertices) / sizeof(float));

    //     VertexBuffer ibo(GL_ELEMENT_ARRAY_BUFFER);
    //     ibo.upload(indices, sizeof(indices) / sizeof(unsigned int));

    //     // attributes
    //     std::vector<VertexAttribute> attrs {
    //         VertexAttribute{
    //             0,
    //             3,
    //             AttributeType::Float,
    //             false,
    //             6,
    //             0,
    //         },
    //         VertexAttribute{
    //             1,
    //             3,
    //             AttributeType::Float,
    //             false,
    //             6,
    //             (void*)( 3 * sizeof(AttributeType::Float)),
    //         },
    //     };

    //     vao.bind();
    //     vao.addVertexBuffer(vbo, attrs)
    //         .setIndexBuffer(ibo);

    // }
};

int main() {
    RectangleApp app(800, 600, "1_01_2_Rectangle");
    app.run();
    return 0;
}