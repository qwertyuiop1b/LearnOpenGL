#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}


// Vertex shader
const char* vertexShaderSrc = R"glsl(
    #version 330 core
    layout(location = 0) in vec2 pos;
    layout(location = 1) in vec3 color;
    layout(location = 2) in float sides;

    out vec3 vColor;
    out float vSides;

    void main() {
        gl_Position = vec4(pos, 0.0, 1.0);
        vColor = color;
        vSides = sides;
    }
)glsl";

// Geometry shader
const char* geometryShaderSrc = R"glsl(
    #version 330 core
    layout(points) in;
    layout(line_strip, max_vertices = 64) out;

    in vec3 vColor[];
    in float vSides[];
    out vec3 fColor;
    
    const float PI = 3.1415926;
    void main() {

        fColor = vColor[0];
        for (int i = 0; i <= vSides[0]; i++) {
            // Angle between each side in radians
            float ang = PI * 2.0 / vSides[0] * i;

            // Offset from center of point (0.3 to accomodate for aspect ratio)
            vec4 offset = vec4(cos(ang) * 0.3, -sin(ang) * 0.4, 0.0, 0.0);
            gl_Position = gl_in[0].gl_Position + offset;
            EmitVertex();
        }

        EndPrimitive();
    }

)glsl";

// Fragment shader
const char* fragmentShaderSrc = R"glsl(
    #version 330 core
    in vec3 fColor;
    out vec4 outColor;


    void main() {
        outColor = vec4(fColor, 1.0);
    }
)glsl";

GLuint createShader(GLenum type, const GLchar* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    return shader;
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "01_2_Geometry", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create glfwwindow" << std::endl;
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

    GLuint vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSrc);
    GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);
    GLuint geometryShader = createShader(GL_GEOMETRY_SHADER, geometryShaderSrc);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, geometryShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    

    float points[] = {
    //  Coordinates  Color             Sides
        -0.45f,  0.45f, 1.0f, 0.0f, 0.0f,  4.0f,
        0.45f,  0.45f, 0.0f, 1.0f, 0.0f,  8.0f,
        0.45f, -0.45f, 0.0f, 0.0f, 1.0f, 16.0f,
        -0.45f, -0.45f, 1.0f, 1.0f, 0.0f, 32.0f
    };

    // create vao vbo
    GLuint vao, vbo;
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while(!glfwWindowShouldClose(window)) {
        process_input(window);

        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glDrawArrays(GL_POINTS, 0, 4);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}