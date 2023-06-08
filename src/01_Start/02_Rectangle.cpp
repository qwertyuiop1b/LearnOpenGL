#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;
const unsigned int INFO_LENGTH = 512;

const char *vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 aPos;

void main() {
  gl_Position = vec4(aPos, 1.0);
})";

const char *fragmentShaderSource = R"(
#version 330 core
out vec4 fragColor;

void main() {
  fragColor = vec4(1.f, 0.f, 0.f, 1.f);
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
          -0.5f, 0.5f, 0.f,
          -0.5f, -0.5f, 0.f,
          0.5f, -0.5f, 0.f,
          0.5f, 0.5f, 0.f,
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

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);  // 这个可在glBindVertexArray(0)之前
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  // 要在glBindVertexArray(0)之后

  // 线框模式
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, .4f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram);
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