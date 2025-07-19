#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "utils/Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


float mixLevel = 0.2f;
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);


int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "09_MultTexture", nullptr, nullptr);
  if (!window) {
    std::cout << "Failed to create window" << std::endl;
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    std::cout << "Failed to initialize glad" << std::endl;
    return -1;
  }

  const char* vertexPath = "shaders/01_shaders/01_09_MultTexture_vs.glsl";
  const char* fragmentPath = "shaders/01_shaders/01_09_MultTexture_fs.glsl";
  Shader shader(vertexPath, fragmentPath);

  stbi_set_flip_vertically_on_load(true);
  int width, height, nrChannels;
  unsigned char* data = stbi_load("textures/container.jpg", &width, &height, &nrChannels, 0);
  std::cout << "nrChannels: " << nrChannels << std::endl;
  if (!data) {
    std::cout << "load failed" << std::endl;
  }
  unsigned int texture1, texture2;
  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  stbi_image_free(data);

  data = stbi_load("textures/awesomeface.png", &width, &height, &nrChannels, 0);
  std::cout << "nrChannels: " << nrChannels << std::endl;
  if (!data) {
    std::cout << "load awesomeface.png failed" << std::endl;
  }

  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);


  // 创建矩形顶点数据和颜色和贴图坐标
  float vertices[] = {
      //     ---- 位置 ----                      ---- 颜色 ----                  - 纹理坐标 -
      0.5f,   0.5f,   0.0f,   1.0f,  0.0f,  0.0f,    1.0f,  1.0f,   // 右上
      0.5f,   -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
      -0.5f, -0.5f,0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
      -0.5f, 0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
  };

//  float vertices[] = {
//      //     ---- 位置 ----                      ---- 颜色 ----                  - 纹理坐标 -
//      0.5f,   0.5f,   0.0f,   1.0f,  0.0f,  0.0f,    0.55f,  0.55f,   // 右上
//      0.5f,   -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   0.45f, 0.55f,   // 右下
//      -0.5f, -0.5f,0.0f,   0.0f, 0.0f, 1.0f,   0.45f, 0.45f,   // 左下
//      -0.5f, 0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.45f, 0.55f    // 左上
//  };

  unsigned int indices[] = {
      0, 1, 2,
      0, 2, 3,
  };

  unsigned int VAO, VBO, EBO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // 0 顶点  1 颜色  2 纹理
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  shader.use();
  shader.setInt("img1", 0);
  shader.setInt("img2", 1);




  while(!glfwWindowShouldClose(window)) {
    processInput(window);
    glClearColor(0.3, 0.2, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    shader.use();
    shader.setFloat("mixLevel", mixLevel);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();

  }
  glDeleteBuffers(1, &EBO);
  glDeleteBuffers(1, &VBO);
  glDeleteVertexArrays(1, &VAO);

  glDeleteTextures(1, &texture1);
  glDeleteTextures(1, &texture2);
  glfwTerminate();

}


void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    mixLevel +=  0.001f;
    if (mixLevel >= 1.0f) {
      mixLevel = 1.0f;
    }
  }
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    mixLevel -= 0.001f;
    if (mixLevel <= 0.0f) {
      mixLevel = 0.0f;
    }
  }
}