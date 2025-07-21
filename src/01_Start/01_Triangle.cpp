#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>


#define MATRICES_LOCATION 1
#define NUM_INSTANCES 1000

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in mat4 aMatrix;

void main() {
  gl_Position = aMatrix * vec4(aPos, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

void main() {
  FragColor = vec4(1.0f, 0.5f, 1.0f, 1.0f);
}
)";


void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);


int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // 启用透明
//  glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GL_TRUE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "01_Triangle", nullptr, nullptr);
  if (window == nullptr) {
    std::cout << "Failed to create glfw window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // 加载opengl函数
  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    glfwTerminate();
    return -1;
  }


  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
  glCompileShader(vertexShader);

  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
    std::cout << "ERROR:SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
    std::cout << "ERROR:SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
  }

  // 链接完成可以删除shader
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  float vertices[9] = {-0.2f, -0.3f, 0.0f,
                      0.2f, -0.3f, 0.0f,
                      0.0f,  0.3f, 0.0f };

  // CPU --> GPU   vertices => vertices buffer
  unsigned int VBO, VAO, MBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &MBO);

  glBindVertexArray(VAO);
  // 指定Buffer的类型 GL_ARRAY_BUFFER
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // 传递到GPU中
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  // 顶点属性以什么方式解析获取数据
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
  // 激活顶点属性
  glEnableVertexAttribArray(0);
  // 解绑
  glBindBuffer(GL_ARRAY_BUFFER, 0);


  glBindBuffer(GL_ARRAY_BUFFER, MBO);
  // initialize model transform
  float mat[NUM_INSTANCES * 16];
  for (int matrix_id = 0; matrix_id < NUM_INSTANCES; matrix_id++) {
    float pos_x = 0.002f *  matrix_id * cos(40*M_PI*matrix_id / NUM_INSTANCES);
    float pos_y = 0.002f *  matrix_id * sin(40*M_PI*matrix_id / NUM_INSTANCES);
    float scale = 0.0004f * matrix_id;

    int i = 16 * matrix_id;
    mat[i+0]  = scale; mat[i+4] = 0.0f;  mat[i+8]  = 0.0f;  mat[i+12] = pos_x; 
    mat[i+1]  = 0.0f;  mat[i+5] = scale; mat[i+9]  = 0.0f;  mat[i+13] = pos_y; 
    mat[i+2]  = 0.0f;  mat[i+6] = 0.0f;  mat[i+10] = scale; mat[i+14] = 0.0f; 
    mat[i+3] =  0.0f;  mat[i+7] = 0.0f;  mat[i+11] = 0.0f;  mat[i+15] = 1.0f; 
  }
  glBufferData(GL_ARRAY_BUFFER, sizeof(mat), mat, GL_DYNAMIC_DRAW);


  for (unsigned int i = 0; i < 4; i++) {
    glEnableVertexAttribArray(MATRICES_LOCATION + i);
    glVertexAttribPointer(MATRICES_LOCATION + i, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(sizeof(float) * i * 4));
    glVertexAttribDivisor(MATRICES_LOCATION + i, 1);
  }



  glBindVertexArray(0);




  while(!glfwWindowShouldClose(window)) {
    // 处理输入
    processInput(window);

    // 渲染
    glClearColor(0.2f, 0.3f, 0.3f, .4f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 3, NUM_INSTANCES);

    // 交换buffer
    glfwSwapBuffers(window);
    glfwPollEvents();
  }



  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);
  glfwTerminate();

  return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}
