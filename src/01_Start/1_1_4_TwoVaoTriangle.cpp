#include "utils/Application.h"
#include "utils/Shader.h"
#include "utils/VertexArray.h"
#include "utils/VertexBuffer.h"
#include <vector>

class TwoVaoTriangle: public Application {
public:
    TwoVaoTriangle(unsigned int width, unsigned int height, const std::string& title)
        : Application(width, height, title) {
        init();
        beforeRender();
    }

    virtual void render() override {
        glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        twoVaoTriangleShader.use();
        vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, 3);
        vao2.bind();
        glDrawArrays(GL_TRIANGLES, 0, 3);

    }

private:
    Shader twoVaoTriangleShader;

    VertexArray vao;
    VertexArray vao2;

    void beforeRender() {
        // create shader from file
        twoVaoTriangleShader.loadFromfile("shaders/01_shaders/1_1_4_TwoVaoTriangle.vert", "shaders/01_shaders/1_1_4_TwoVaoTriangle.frag");
        // create vao vbo ebo
        float vertices[] {
            -0.5f, 0.5f, 0.f,
            0.f, 0.f, 0.f,
            -0.5f, -0.5f, 0.f,
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


        // vao2
        vao2.create();
        vao2.bind();
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
        vao2.addVertexBuffer(vbo2, attrs2);
        vbo2.unbind();
        vao2.unbind();
    }
};

int main() {
    TwoVaoTriangle app(800, 600, "1_1_4_TwoVaoTriangle");
    app.run();
    return 0;
}