#ifndef OPENGL_UTILS_APPLICATION_H
#define OPENGL_UTILS_APPLICATION_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class Application {
private:
    GLFWwindow* mWindow;
    unsigned int mWidth, mHeight;
    std::string mTitle;
    bool mIsRunning;

    static void keyCallbackWrapper(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void framebufferSizeCallbackWrapper(GLFWwindow* window, int width, int height);


public:
    Application(unsigned int width, unsigned int height, const std::string& title);
    ~Application() = default;
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    void run();

    virtual void render();

    void update();

    void cleanup();

    void requestExit() { mIsRunning = false; }

    unsigned int getWidth() const { return mWidth; }
    unsigned int getHeight() const { return mHeight; }
private:
    void checkIsRunning();

    void init(unsigned int glVersionMajor = 3, unsigned int glVersionMinor = 3);

};


#endif