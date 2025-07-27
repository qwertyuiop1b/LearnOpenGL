#include "utils/Application.h"
#include "utils/Shader.h"
#include "utils/VertexArray.h"
#include "utils/VertexBuffer.h"
#include <vector>

class TwoTriangle: public Application {
public:
    TwoTriangle(unsigned int width, unsigned int height, const std::string& title)
        : Application(width, height, title) {
        init();
        beforeRender();
    }

    virtual void render() override {
        glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        twoTriangleShader.use();
        vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

private:
    Shader twoTriangleShader;

    VertexArray vao;

    void beforeRender() {
        // create shader from file
        twoTriangleShader.loadFromfile("shaders/01_shaders/1_1_3_TwoTriangle.vert", "shaders/01_shaders/1_1_3_TwoTriangle.frag");
        // create vao vbo ebo
        float vertices[] {
            -0.5f, 0.5f, 0.f,
            0.f, 0.f, 0.f,
            -0.5f, -0.5f, 0.f,
            0.f, 0.f, 0.f,
            0.5f, 0.5f, 0.f,
            0.5f, -0.5f, 0.f

        };

        // vao
        vao.create();
        vao.bind();

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
        vao.addVertexBuffer(vbo, attrs);
        vbo.unbind();
        vao.unbind();
    }
};

int main() {
    TwoTriangle app(800, 600, "1_1_3_TwoTriangle");
    app.run();
    return 0;
}