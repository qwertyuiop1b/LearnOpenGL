#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
// 更改为渲染nv12
const unsigned int WIDTH = 1920;
const unsigned int HEIGHT = 1080;
const unsigned int INFO_LENGTH = 512;

const char *vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
out vec2 vTexCoord;

void main() {
  gl_Position = vec4(aPos, 1.0);
  vTexCoord = aTexCoord;
})";

const char *fragmentShaderSource = R"(
#version 330 core
precision mediump float;
uniform sampler2D uTextureY;
uniform sampler2D uTextureUV;
out vec4 fragColor;
in vec2 vTexCoord;

void main() {
    // 获取Y分量
    float Y = texture(uTextureY, vTexCoord).r;

    // 获取U和V分量
//    vec2 uv = vTexCoord; // UV坐标需要缩放为原始图像的一半
    float U = texture(uTextureUV, vTexCoord).r - 0.5;
    float V = texture(uTextureUV, vTexCoord).g - 0.5;

    // YUV 转 RGB 公式
    float R = Y + 1.402 * V;
    float G = Y - 0.34414 * U - 0.71414 * V;
    float B = Y + 1.772 * U;

    fragColor = vec4(R, G, B, 1.0);
})";


void frame_buffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "02_Rectangle", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    char infoLog[INFO_LENGTH];
    int success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, INFO_LENGTH, nullptr, infoLog);
        std::cout << "ERROR: Failed to compile vertex shader\n" << infoLog << std::endl;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, INFO_LENGTH, nullptr, infoLog);
        std::cout << "ERROR: Failed to compile fragment shader\n" << infoLog << std::endl;
    }

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    const float vertices[] = {
            -1.0f, 1.0f, 0.f, 0.0f, 0.0f,
            -1.0f, -1.0f, 0.f, 0.0f, 1.0f,
            1.0f, -1.0f, 0.f, 1.0f, 1.0f,
            1.0f, 1.0f, 0.f, 1.0f, 0.f,
    };

    const unsigned int indices[] = {
            0, 1, 2,
            0, 3, 2,
    };

    unsigned int VBO, EBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    glBindBuffer(GL_ARRAY_BUFFER, 0);  // 这个可在glBindVertexArray(0)之前
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  // 要在glBindVertexArray(0)之后


    // read yuv file
    const uint32_t image_width = 1920;
    const uint32_t image_height = 1080;
    const uint32_t size = image_width * image_height * 3 / 2;

    std::ifstream file("../textures/dump_out_raw_image_nv12_1920x1080.yuv", std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "ERROR: Could not open file " << std::endl;
    }

    auto *yuvData = new unsigned char[size];
    file.read(reinterpret_cast<char*>(yuvData), size);
    file.close();


    GLuint textureY, textureUV;
    glGenTextures(1, &textureY);
    glBindTexture(GL_TEXTURE_2D, textureY);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, image_width, image_height, 0, GL_RED, GL_UNSIGNED_BYTE, yuvData);


    glGenTextures(1, &textureUV);
    glBindTexture(GL_TEXTURE_2D, textureUV);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, image_width / 2, image_height / 2, 0, GL_RG, GL_UNSIGNED_BYTE,
                 yuvData + (image_width * image_height));

    glBindTexture(GL_TEXTURE_2D, 0);

    // 线框模式
//  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, .4f);
        glClear(GL_COLOR_BUFFER_BIT);


        glUseProgram(shaderProgram);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureY);
        glUniform1i(glGetUniformLocation(shaderProgram, "uTextureY"), 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureUV);
        glUniform1i(glGetUniformLocation(shaderProgram, "uTextureUV"), 1);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();


    return 0;
}

void frame_buffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}