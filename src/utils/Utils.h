//
// Created by minieye on 2023/6/21.
//

#ifndef OPENGL_UTILS_H
#define OPENGL_UTILS_H

#include <GLFW/glfw3.h>


class Utils {
public:
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }
};


#endif //OPENGL_UTILS_H
