#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "utils/Shader.h"

#include "utils/stb_define.h"

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

void processInput(GLFWwindow* window);

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "10_Transformation", nullptr, nullptr);
  if (!window) {
    std::cout << "Failed to create window" << std::endl;
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to load gl";
    return -1;
  }

  const char* vertPath = "shaders/01_shaders/01_10_Transformation_vs.glsl";
  const char* fragmentPath = "shaders/01_shaders/01_10_Transformation_fs.glsl";
  Shader shader(vertPath, fragmentPath);


  stbi_set_flip_vertically_on_load(true);
  // 加载贴图
  int width, height, nrChannels;
  unsigned char* data = stbi_load("../textures/container.jpg", &width, &height, &nrChannels, 0);
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

  unsigned char* data2 = stbi_load("../textures/awesomeface.png", &width, &height, &nrChannels, 0);
  if (!data2) {
    std::cout << "load failed 2" <<std::endl;
  }

  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
  stbi_image_free(data2);


  float vertices[] = {
            // 顶点坐标                       纹理坐标
          -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
          -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
          0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
          0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
  };

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

  glVertexAttribPointer(0, 3, GL_FLOAT, false, 5 * sizeof(float), (void *) 0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, false, 5 * sizeof(float), (void *) (3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

//  glm::vec4 testPoint(1.0f, 0.0f, 0.0f, 1.0f);
//  glm::mat4 transformation = glm::mat4(1.0f);
//  transformation = glm::rotate(transformation, glm::radians(90.0f), glm::vec3(0, 0, 1));
//  transformation = glm::translate(transformation, glm::vec3(0.3, 0, 0));
//  testPoint = transformation * testPoint;
//  std::cout << testPoint.x << " " << testPoint.y << " " << testPoint.z << std::endl;




  shader.use();
  shader.setInt("img1", 0);
  shader.setInt("img2", 1);


  while(!glfwWindowShouldClose(window)) {
    processInput(window);
    glClearColor(0.3f, 0.2f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);


    shader.use();
    unsigned int transformLoc = glGetUniformLocation(shader.ID, "transformation");
    glm::mat4 transformation = glm::mat4(1.0f);
    // 交换两个变换的顺序
    transformation = glm::rotate(transformation, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
    transformation = glm::translate(transformation, glm::vec3(0.5f, -0.5f, 0.0f));
    glUniformMatrix4fv(transformLoc, 1, false, glm::value_ptr(transformation));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();

  }


  

  return 0;
}


void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}