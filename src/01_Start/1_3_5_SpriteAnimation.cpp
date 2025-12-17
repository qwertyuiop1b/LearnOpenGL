#include <memory>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "utils/Window.h"
#include "utils/FPS.h"
#include "sprite/SpriteSheet.h"
#include "sprite/Animation.h"
#include "sprite/SpriteRenderer.h"

/**
 * Sprite 动画示例
 * 
 * 展示如何使用 Sprite 动画系统：
 * - 加载精灵图
 * - 定义帧和动画
 * - 播放和控制动画
 * - ImGui 控制面板
 */
class SpriteAnimationDemo {
private:
    std::unique_ptr<Window> window;
    std::unique_ptr<FPS> fps;
    std::unique_ptr<Sprite::SpriteSheet> spriteSheet;
    std::unique_ptr<Sprite::SpriteRenderer> renderer;
    
    // UI 状态
    float animSpeed = 1.0f;
    bool animLoop = true;
    int selectedAnim = 0;
    const char* animNames[2] = {"Fire", "Fire Fast"};
    
public:
    SpriteAnimationDemo(unsigned int width, unsigned int height, const std::string& title)
        : window(std::make_unique<Window>(width, height, title))
        , fps(std::make_unique<FPS>(60))
    {
        initImGui();
        init();
    }
    
    ~SpriteAnimationDemo() {
        cleanupImGui();
    }
    
    void run() {
        while (!window->shouldClose()) {
            // 更新
            fps->update();
            float deltaTime = fps->getDeltaTime();
            
            window->pollEvents();
            
            if (renderer) {
                renderer->update(deltaTime);
            }
            
            // 渲染
            glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            
            // 启用混合（用于透明度）
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            
            // 渲染精灵
            if (renderer && renderer->getCurrentAnimation()) {
                // 计算精灵位置（屏幕中心）
                glm::vec2 position(0.0f, 0.0f);
                glm::vec2 size(2.0f, 2.0f);  // 2x2 单位大小
                
                renderer->render(position, size, 0.0f, glm::vec3(1.0f));
            }
            
            // 渲染 UI
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            
            renderUI();
            
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            
            window->swapBuffer();
        }
    }
    
private:
    void initImGui() {
        // 初始化 ImGui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        
        // 设置 ImGui 样式
        ImGui::StyleColorsDark();
        
        // 初始化 ImGui 后端
        ImGui_ImplGlfw_InitForOpenGL(window->getGLFWWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 330");
        
        std::cout << "ImGui initialized" << std::endl;
    }
    
    void cleanupImGui() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
    
    void init() {
        std::cout << "=== Sprite Animation Demo ===" << std::endl;
        std::cout << "Initializing..." << std::endl;
        
        // 加载精灵图 - 尝试多个可能的路径
        const char* possiblePaths[] = {
            "../assets/textures/fire_frame.jpg",      // 从 build/ 目录运行
            "assets/textures/fire_frame.jpg",         // 从项目根目录运行
            "./assets/textures/fire_frame.jpg"        // 备选
        };
        
        std::string texturePath;
        for (const char* path : possiblePaths) {
            // 尝试打开文件检查是否存在
            FILE* f = fopen(path, "rb");
            if (f) {
                fclose(f);
                texturePath = path;
                std::cout << "Found texture at: " << path << std::endl;
                break;
            }
        }
        
        if (texturePath.empty()) {
            std::cerr << "Error: Could not find fire_frame.jpg!" << std::endl;
            std::cerr << "Tried paths:" << std::endl;
            for (const char* path : possiblePaths) {
                std::cerr << "  - " << path << std::endl;
            }
            return;
        }
        
        spriteSheet = std::make_unique<Sprite::SpriteSheet>(texturePath);
        
        // 定义帧（火焰动画精灵图）
        // 精灵图布局：1920x1920，6x6 网格，每帧 320x320
        // 总共 36 帧的火焰燃烧动画
        
        std::cout << "Defining frames..." << std::endl;
        
        // 使用网格方法添加所有 36 帧
        // 参数：startX, startY, frameWidth, frameHeight, cols, rows
        spriteSheet->addFrameGrid(0, 0, 320, 320, 6, 6);
        
        std::cout << "Fire sprite sheet: 1920x1920, 6x6 grid, 320x320 per frame" << std::endl;
        
        std::cout << "Total frames defined: " << spriteSheet->getFrameCount() << std::endl;
        
        if (!spriteSheet->isValid()) {
            std::cerr << "Error: Failed to initialize sprite sheet!" << std::endl;
            return;
        }
        
        // 创建渲染器
        renderer = std::make_unique<Sprite::SpriteRenderer>(spriteSheet.get());
        
        // 定义动画
        std::cout << "Creating animations..." << std::endl;
        
        // 火焰动画：使用所有 36 帧，15 FPS
        std::vector<int> fireFrames;
        for (int i = 0; i < 36; ++i) {
            fireFrames.push_back(i);
        }
        
        Sprite::Animation fireAnim("fire", fireFrames, 15.0f);
        fireAnim.setLooping(true);
        renderer->addAnimation(fireAnim);
        
        // 快速火焰动画：使用偶数帧，20 FPS
        std::vector<int> fastFireFrames;
        for (int i = 0; i < 36; i += 2) {
            fastFireFrames.push_back(i);
        }
        
        Sprite::Animation fastFireAnim("fire_fast", fastFireFrames, 20.0f);
        fastFireAnim.setLooping(true);
        renderer->addAnimation(fastFireAnim);
        
        // 设置初始动画
        renderer->setAnimation("fire");
        
        std::cout << "Initialization complete!" << std::endl;
        std::cout << "\nControls:" << std::endl;
        std::cout << "- Use ImGui panel to control animation" << std::endl;
        std::cout << "- Switch between Fire (36 frames) and Fire Fast (18 frames)" << std::endl;
        std::cout << "- Adjust speed (0.1x - 5.0x)" << std::endl;
        std::cout << "- Toggle loop and pause/play" << std::endl;
        std::cout << "============================\n" << std::endl;
    }
    
    void renderUI() {
        // 创建控制面板
        ImGui::Begin("Sprite Animation Control", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Animation System");
        ImGui::Separator();
        
        if (renderer && renderer->getCurrentAnimation()) {
            auto* anim = renderer->getCurrentAnimation();
            
            // 动画信息
            ImGui::Text("Current Animation: %s", anim->getName().c_str());
            ImGui::Text("Current Frame: %d / %zu", 
                        anim->getCurrentFrame(), 
                        anim->getFrameCount());
            ImGui::Text("FPS: %.1f", anim->getFPS());
            
            ImGui::Separator();
            
            // 动画切换
            ImGui::Text("Switch Animation:");
            if (ImGui::RadioButton("Fire (36 frames)", selectedAnim == 0)) {
                selectedAnim = 0;
                renderer->setAnimation("fire");
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Fire Fast (18 frames)", selectedAnim == 1)) {
                selectedAnim = 1;
                renderer->setAnimation("fire_fast");
            }
            
            ImGui::Separator();
            
            // 速度控制
            if (ImGui::SliderFloat("Speed", &animSpeed, 0.1f, 5.0f, "%.2fx")) {
                anim->setSpeed(animSpeed);
            }
            
            // 循环控制
            if (ImGui::Checkbox("Loop", &animLoop)) {
                anim->setLooping(animLoop);
            }
            
            ImGui::Separator();
            
            // 播放控制
            if (anim->isPaused()) {
                if (ImGui::Button("Play", ImVec2(100, 0))) {
                    renderer->play();
                }
            } else {
                if (ImGui::Button("Pause", ImVec2(100, 0))) {
                    renderer->pause();
                }
            }
            
            ImGui::SameLine();
            
            if (ImGui::Button("Reset", ImVec2(100, 0))) {
                anim->reset();
            }
            
            ImGui::Separator();
            
            // 状态信息
            ImGui::Text("Status:");
            ImGui::BulletText("Paused: %s", anim->isPaused() ? "Yes" : "No");
            ImGui::BulletText("Finished: %s", anim->isFinished() ? "Yes" : "No");
            ImGui::BulletText("Looping: %s", anim->isLooping() ? "Yes" : "No");
            
        } else {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "No animation loaded!");
        }
        
        ImGui::Separator();
        
        // 帮助信息
        if (ImGui::CollapsingHeader("Help")) {
            ImGui::TextWrapped(
                "Fire Sprite Animation Demo:\n\n"
                "This demo displays a 36-frame fire animation from a 6x6 sprite sheet.\n\n"
                "Sprite Sheet Info:\n"
                "- File: textures/fire_frame.jpg\n"
                "- Size: 1920x1920 pixels\n"
                "- Grid: 6x6 (36 frames)\n"
                "- Frame Size: 320x320 pixels\n\n"
                "Controls:\n"
                "- Fire: All 36 frames at 15 FPS\n"
                "- Fire Fast: Every other frame (18 frames) at 20 FPS\n"
                "- Speed slider: 0.1x to 5.0x\n"
                "- Play/Pause/Reset buttons\n"
                "- Loop toggle"
            );
        }
        
        ImGui::End();
        
        // FPS 显示
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
        ImGui::Begin("Performance", nullptr, 
                     ImGuiWindowFlags_NoTitleBar | 
                     ImGuiWindowFlags_NoResize | 
                     ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("FPS: %.1f (%.2f ms)", 
                    1.0f / fps->getDeltaTime(), 
                    fps->getDeltaTime() * 1000.0f);
        ImGui::End();
    }
};

int main() {
    try {
        SpriteAnimationDemo demo(800, 600, "Sprite Animation Demo");
        demo.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}

