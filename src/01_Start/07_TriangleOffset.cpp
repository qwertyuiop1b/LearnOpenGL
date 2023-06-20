#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "utils/Shader.h"
#include <iostream>

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

void frame_buffer_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);


int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "07_TriangleOffset", nullptr, nullptr);
  if (!window) {
    std::cout << "Failed to create window" << std::endl;
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, frame_buffer_callback);
  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD";
    return -1;
  }

  const char* vertexPath = "../shaders/01_shaders/01_07_TriangleOffset_vs.glsl";
  const char* fragmentPath = "../shaders/01_shaders/01_07_TriangleOffset_fs.glsl";
  Shader shader(vertexPath, fragmentPath);

  // 创建顶点数据和颜色
  float vertices[] = {
          0.f, 0.5f, 0.f,  1.0f, 1.0f, 1.0f,
          -0.25f, 0.f, 0.f, 1.0f, 0.f, 0.f,
          0.25f, 0.f, 0.f, 0.f,1.f,0.f,
  };

  unsigned int VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);



  while(!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.3f, 0.3f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    shader.use();
    auto time = static_cast<float>(glfwGetTime());
    shader.setFloat2("offset", 0.3f * sin(time), 0.3f * cos(time));
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);


}


void frame_buffer_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}