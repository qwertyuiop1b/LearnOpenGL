#ifndef IMGUI_MANAGER_H
#define IMGUI_MANAGER_H

#include <GLFW/glfw3.h>
#include <memory>

class ImGuiManager {
public:
    explicit ImGuiManager(GLFWwindow* window);
    ~ImGuiManager();
    
    void newFrame();
    void render();
    
private:
    void init(GLFWwindow* window);
    void shutdown();
    
    bool initialized;
};

#endif // IMGUI_MANAGER_H
