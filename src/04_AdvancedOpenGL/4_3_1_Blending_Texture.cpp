#include "glm/fwd.hpp"
#include "utils/Application.h"
#include "utils/OribitCamera.h"
#include "utils/Shader.h"
#include "utils/Texture.h"
#include "utils/VertexArray.h"
#include "utils/VertexBuffer.h"
#include <vector>
#include <glm/glm.hpp>



class Blending: public Application {
public:
    bool dragging = false;
    double lastX, lastY;
    double curX, curY;

    Blending(unsigned int width, unsigned int height, const std::string& title): Application(width, height, title) {
        init();
        setup();

        lastX = width / 2.0;
        lastY = height / 2.0;

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        // grass location
        vegetation.push_back(glm::vec3(-1.5f,  0.0f, -0.48f));
        vegetation.push_back(glm::vec3( 1.5f,  0.0f,  0.51f));
        vegetation.push_back(glm::vec3( 0.0f,  0.0f,  0.7f));
        vegetation.push_back(glm::vec3(-0.3f,  0.0f, -2.3f));
        vegetation.push_back(glm::vec3( 0.5f,  0.0f, -0.6f));
    }

    virtual void render() override {
        const auto leftMouseBtnState = glfwGetMouseButton(getWindow(), GLFW_MOUSE_BUTTON_LEFT);
        if (leftMouseBtnState == GLFW_PRESS) {
            if (!dragging) {
                glfwGetCursorPos(getWindow(), &lastX, &lastY);
                dragging = true;
            }
            glfwGetCursorPos(getWindow(), &curX, &curY);
            const auto deltaX = curX - lastX;
            const auto deltaY = curY - lastY;
            camera.rotateAzimuth(glm::radians(static_cast<float>(deltaX) * 0.5f));
            camera.rotatePolar(glm::radians(static_cast<float>(deltaY) * 0.5f));
            lastX = curX;
            lastY = curY;
        } else {
            dragging = false;
        }

        glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // cube
        shader.use();
        glm::mat4  model = glm::mat4(1.0f);
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(getWidth()) / static_cast<float>(getHeight()), 0.1f, 100.0f);
        shader.setMatrix4("view", view);
        shader.setMatrix4("projection", projection);
        cubeVao.bind();
        cubeTexture.bind();
        model = glm::translate(model, glm::vec3(-1.f, 0.f, -1.f));
        shader.setMatrix4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.f, 0.f, 0.f));
        shader.setMatrix4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        cubeVao.unbind();

        // floor
        planeVao.bind();
        planeTexture.bind();
        model = glm::mat4(1.0f);
        shader.setMatrix4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        planeVao.unbind();


        quadShader.use();
        quadShader.setMatrix4("view", view);
        quadShader.setMatrix4("projection", projection);
        for (auto &pos : vegetation) {
            quadVao.bind();
            grassTexture.bind();
            model = glm::mat4(1.0f);
            model = glm::translate(model, pos);
            shader.setMatrix4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

    }
private:
    Shader shader;
    Shader quadShader;

    VertexArray cubeVao;
    VertexArray planeVao;
    VertexArray quadVao;

    Texture cubeTexture;
    Texture planeTexture;
    Texture grassTexture;

    std::vector<glm::vec3> vegetation;

    OribitCamera camera{glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f), 10.0f, 1.0f, 0.f, 0.f};

    void setup() {
        float cubeVertices[] = {
            // positions          // texture Coords
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
        float planeVertices[] = {
            // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
            5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
            -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
            -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

            5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
            -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
            5.0f, -0.5f, -5.0f,  2.0f, 2.0f
        };
        float quadVertices[] = {
            // positions          // texture Coords
            0.0f,  0.5f,  0.0f,  0.0f,  1.0f,
            0.0f, -0.5f,  0.0f,  0.0f,  0.0f,
            1.0f, -0.5f,  0.0f,  1.0f,  0.0f,

            0.0f,  0.5f,  0.0f,  0.0f,  1.0f,
            1.0f, -0.5f,  0.0f,  1.0f,  0.0f,
            1.0f,  0.5f,  0.0f,  1.0f,  1.0f
        };

        cubeVao.create();
        cubeVao.bind();
        VertexBuffer cubeVbo(GL_ARRAY_BUFFER);
        cubeVbo.bind();
        cubeVbo.upload(cubeVertices, sizeof(cubeVertices) / sizeof(float));
        std::vector<VertexAttribute> cubeAttributes = {
            {0, 3, AttributeType::Float, false, 5 * sizeof(float), (const void*)0},
            {1, 2, AttributeType::Float, false, 5 * sizeof(float), (const void*)(3 * sizeof(float))}
        };
        cubeVao.addVertexBuffer(cubeVbo, cubeAttributes);
        cubeVao.unbind();
        cubeVbo.unbind();

        planeVao.create();
        planeVao.bind();
        VertexBuffer planeVbo(GL_ARRAY_BUFFER);
        planeVbo.bind();
        planeVbo.upload(planeVertices, sizeof(planeVertices) / sizeof(float));
        std::vector<VertexAttribute> planeAttributes = {
            {0, 3, AttributeType::Float, false, 5 * sizeof(float), (const void*)0},
            {1, 2, AttributeType::Float, false, 5 * sizeof(float), (const void*)(3 * sizeof(float))}
        };
        planeVao.addVertexBuffer(planeVbo, planeAttributes);
        planeVao.unbind();
        planeVbo.unbind();

        quadVao.create();
        quadVao.bind();
        VertexBuffer quadVbo;
        quadVbo.bind();
        quadVbo.upload(quadVertices, sizeof(quadVertices) / sizeof(float));
        std::vector<VertexAttribute> quadAttributes =  {
            {0, 3, AttributeType::Float, false, 5 * sizeof(float), (const void*)0},
            {1, 2, AttributeType::Float, false, 5 * sizeof(float), (const void*)(3 * sizeof(float))}
        };
        quadVao.addVertexBuffer(quadVbo, quadAttributes);
        quadVao.unbind();
        quadVbo.unbind();

        // shader
        shader.loadFromfile("shaders/04_shaders/4_1_1_depth_test.vert", "shaders/04_shaders/4_1_1_depth_test.frag");
        // quad shader
        quadShader.loadFromfile("shaders/04_shaders/4_3_1_blending_quad.vert", "shaders/04_shaders/4_3_1_blending_quad.frag");

        // load textures;
        cubeTexture.loadFromFile("textures/marble.jpg");
        planeTexture.loadFromFile("textures/metal.png");
        grassTexture.loadFromFile("textures/grass.png");
        grassTexture.setParameter(GL_TEXTURE_WRAP_S, static_cast<GLuint>(GL_CLAMP_TO_EDGE));
        grassTexture.setParameter(GL_TEXTURE_WRAP_T, static_cast<GLuint>(GL_CLAMP_TO_EDGE));

        shader.use();
        shader.setInt("texture1", 0);

        quadShader.use();
        quadShader.setInt("quadTexture", 0);
    }
};

int main() {
    Blending app(800, 600, "Blending Texture");
    app.run();
    return 0;
}