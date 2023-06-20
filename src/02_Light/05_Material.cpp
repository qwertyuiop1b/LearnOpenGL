#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "utils/Shader.h"
#include "utils/Camera.h"

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 6.0f));
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;


// time
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(0.f, 1.5f, 0.f);


int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "02_BaseLight", nullptr, nullptr);
  if (!window) {
    std::cout << "Failed to create window" << std::endl;
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

  if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress))) {
    std::cout << "Failed to load gl" << std::endl;
    return -1;
  }

  const char* vertexPath = "../shaders/02_shaders/02_05_Material_vs.glsl";
  const char* fragmentPath = "../shaders/02_shaders/02_05_Material_fs.glsl";

  const char* lightFragmentPath = "../shaders/02_shaders/02_01_Color_Light_fs.glsl";

  Shader shader(vertexPath, fragmentPath);
  Shader lightShader(vertexPath, lightFragmentPath);


  // 顶点和法线
  float vertices[] = {
          -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
          0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
          0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
          0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
          -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
          -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

          -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
          0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
          0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
          0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
          -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
          -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

          -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
          -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
          -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
          -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
          -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
          -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

          0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
          0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
          0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
          0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
          0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
          0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

          -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
          0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
          0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
          0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
          -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
          -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

          -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
          0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
          0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
          0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
          -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
          -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
  };


  unsigned int VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(float), (void *) 0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * sizeof(float), (void *) (3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glBindVertexArray(0);

  unsigned int lightVAO;
  glGenVertexArrays(1, &lightVAO);
  glBindVertexArray(lightVAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);

  glEnable(GL_DEPTH_TEST);

  while(!glfwWindowShouldClose(window)) {
    auto currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(window);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    shader.use();
    /**
     * 环境光颜色、漫反射跟物体颜色一致
     * 高光是光的颜色的比例
     * 反光度
     */
    shader.setFloat3("material.ambient", 1.0f, 0.5f, 0.31f);
    shader.setFloat3("material.diffuse", 1.0f, 0.5f, 0.31f);
    shader.setFloat3("material.specular", 0.5f, 0.5f, 0.5f);
    shader.setFloat("material.shininess", 32.0f);

    shader.setFloat3("light.position", lightPos);

//    shader.setFloat3("light.ambient", 0.2f, 0.2f, 0.2f);
//    shader.setFloat3("light.diffuse", 0.5f, 0.5f, 0.5f);
//    shader.setFloat3("light.specular", 1.0f, 1.0f, 1.0f);

    glm::vec3 lightColor;
    lightColor.x = sin(currentFrame * 2.0f);
    lightColor.y = sin(currentFrame * 0.7f);
    lightColor.z = sin(currentFrame * 1.3f);

    glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
    glm::vec3 ambientColor = lightColor * glm::vec3(0.2f);
    glm::vec3 specularColor = lightColor * glm::vec3(1.0f);

    shader.setFloat3("light.ambient", ambientColor);
    shader.setFloat3("light.diffuse", diffuseColor);
    shader.setFloat3("light.specular", specularColor);

    shader.setFloat3("viewPos", camera.Position);

    glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera.Zoom), float(WIDTH)/float(HEIGHT), 0.1f, 100.0f);
    glm::mat4 viewMatrix = camera.GetViewMatrix();
    shader.setMatrix4("projectionMatrix", projectionMatrix);
    shader.setMatrix4("viewMatrix", viewMatrix);

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(60.0f), glm::vec3(1.0f, 1.0f, 0.0f));

    shader.setMatrix4("modelMatrix", modelMatrix);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    lightShader.use();
    lightShader.setMatrix4("projectionMatrix", projectionMatrix);
    lightShader.setMatrix4("viewMatrix", viewMatrix);
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, lightPos);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
    lightShader.setMatrix4("modelMatrix", modelMatrix);
    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glDeleteBuffers(1, &VBO);
  glDeleteVertexArrays(1, &lightVAO);
  glDeleteVertexArrays(1, &VAO);
  glfwTerminate();
  return 0;
}


void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouseCallback(GLFWwindow* window, double xposIn, double yposIn){
  auto xpos = static_cast<float>(xposIn);
  auto ypos = static_cast<float>(yposIn);
  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }
  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
  lastX = xpos;
  lastY = ypos;
  camera.ProcessMouseMovement(xoffset, yoffset);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset){
  camera.ProcessMouseScroll(static_cast<float>(yoffset));
}