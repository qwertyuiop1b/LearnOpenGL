#include <string>
#include <memory>
#include "utils/VertexBuffer.h"
#include "utils/Window.h"
#include "utils/Shader.h"
#include "utils/VertexArray.h"
#include "utils/FPS.h"


class GlslPolar {
public:
    GlslPolar(unsigned int width, unsigned int height, const std::string& title)
        : window(std::make_unique<Window>(width, height, title))
        , shader(std::make_unique<Shader>("shaders/01_shaders/1_2_4_GlslPolar.vert", "shaders/01_shaders/1_2_4_GlslPolar.frag"))
        , vao(std::make_unique<VertexArray>())
        , fps(std::make_unique<FPS>(60)) {
        init();
    }
    void run() {
        float progress = 0.0f;
        float progressSpeed = 0.3f; // 进度增加速度（每秒）
        
        while(!window->shouldClose()) {
            fps->update();
            window->pollEvents();
            
            // 使用 deltaTime 更新进度（与帧率无关）
            float deltaTime = fps->getDeltaTime();
            progress += progressSpeed * deltaTime;
            if (progress > 1.0f) progress = 0.0f;
            
            glClearColor(0.0, 0.0, 0.0, 1.0);
            glClear(GL_COLOR_BUFFER_BIT);
            
            shader->use();
            shader->setFloat("uProgress", progress);
            vao->bind();
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            window->swapBuffer();
        }
    }
private:
    std::unique_ptr<Window> window;
    std::unique_ptr<Shader> shader;
    std::unique_ptr<VertexArray> vao;
    std::unique_ptr<FPS> fps;

    void init() {
        float vertices[] = {
            -1, 1, 0,
            -1, -1, 0,
            1, -1, 0,
            1, 1, 0
        };

        unsigned int indices[] = {
            0, 1, 2,
            0, 2, 3,
        };

        vao->bind();
        VertexBuffer vbo(GL_ARRAY_BUFFER);
        vbo.upload(vertices, sizeof(vertices) / sizeof(float));
        std::vector<VertexAttribute> attrs {
            VertexAttribute{
                0,
                3,
                AttributeType::Float,
                false,
            }
        };
        vao->addVertexBuffer(vbo, attrs);

        VertexBuffer ebo(GL_ELEMENT_ARRAY_BUFFER);
        ebo.upload(indices, sizeof(indices) / sizeof(unsigned int));
        vao->setIndexBuffer(ebo);
        
        vao->unbind();
        vbo.unbind();
        ebo.unbind();

        // 启用混合（Blending）
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
};


int main() {
    GlslPolar app(800, 600, "1_2_4_GlslPolar");
    app.run();
    return 0;
}