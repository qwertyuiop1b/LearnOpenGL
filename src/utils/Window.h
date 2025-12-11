#ifndef OPENGL_UTILS_WINDOW_H
#define OPENGL_UTILS_WINDOW_H

#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


class Window {
public:
    Window(unsigned int width, unsigned int height, const std::string& title);
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    ~Window();

    void init(unsigned int glVersionMajor = 3, unsigned int glVersionMinor = 3);

    bool shouldClose() const { return glfwWindowShouldClose(mWindow); };

    inline void swapBuffer() { glfwSwapBuffers(mWindow); }

    inline void pollEvents() { glfwPollEvents(); }

    inline unsigned int getWidth() const { return mWidth; }

    inline unsigned int getHeight() const { return mHeight; }

private:
    GLFWwindow* mWindow;
    unsigned int mWidth, mHeight;
    std::string mTitle;


    void checkIsRequestExit();
    static void keyCallbackWrapper(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void framebufferSizeCallbackWrapper(GLFWwindow* window, int width, int height);
};

#endif