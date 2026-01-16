#include "ImGuiManager.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

ImGuiManager::ImGuiManager(GLFWwindow* window) : initialized(false) {
    init(window);
}

ImGuiManager::~ImGuiManager() {
    shutdown();
}

void ImGuiManager::init(GLFWwindow* window) {
    // 初始化 ImGui 上下文
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    
    // 设置 ImGui 样式
    ImGui::StyleColorsDark();
    
    // 初始化 ImGui 的 GLFW 和 OpenGL3 实现
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    
    initialized = true;
}

void ImGuiManager::shutdown() {
    if (initialized) {
        // 清理 ImGui 的实现
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        
        initialized = false;
    }
}

void ImGuiManager::newFrame() {
    if (initialized) {
        // 开始新的 ImGui 帧
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }
}

void ImGuiManager::render() {
    if (initialized) {
        // 渲染 ImGui 界面
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}
