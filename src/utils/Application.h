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

    static void keyCallbackWrapper(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void framebufferSizeCallbackWrapper(GLFWwindow* window, int width, int height);


public:
    Application(unsigned int width, unsigned int height, const std::string& title);
    ~Application();
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    void run();

    void requestExit();

    unsigned int getWidth() const { return mWidth; }

    unsigned int getHeight() const { return mHeight; }

    GLFWwindow* getWindow() const { return mWindow; }

protected:
    void init(unsigned int glVersionMajor = 3, unsigned int glVersionMinor = 3);

    void checkIsRequestExit();

    void update();

    void cleanup();

    virtual void render();

};


#endif