#include <memory>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "imgui.h"
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
    const char* animNames[2] = {"Idle", "Walk"};
    
public:
    SpriteAnimationDemo(unsigned int width, unsigned int height, const std::string& title)
        : window(std::make_unique<Window>(width, height, title))
        , fps(std::make_unique<FPS>(60))
    {
        init();
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
            renderUI();
            
            window->swapBuffer();
        }
    }
    
private:
    void init() {
        std::cout << "=== Sprite Animation Demo ===" << std::endl;
        std::cout << "Initializing..." << std::endl;
        
        // 加载精灵图
        // 注意：你需要提供自己的精灵图文件
        // 这里使用一个示例路径，如果文件不存在会创建测试纹理
        spriteSheet = std::make_unique<Sprite::SpriteSheet>("textures/sprite_sheet.png");
        
        if (!spriteSheet->isValid()) {
            std::cerr << "Warning: Failed to load sprite sheet!" << std::endl;
            std::cerr << "Creating test animation with placeholder..." << std::endl;
            // 仍然可以继续，使用单帧
        }
        
        // 定义帧（不规则布局示例）
        // 假设精灵图布局：
        // Row 0: Idle 动画（4 帧，每帧 64x64）
        // Row 1: Walk 动画（6 帧，每帧 64x64）
        
        // 方式1：手动添加每一帧
        std::cout << "Defining frames..." << std::endl;
        
        // Idle 动画帧 (Frame 0-3)
        spriteSheet->addFrame(0, 0, 64, 64);
        spriteSheet->addFrame(64, 0, 64, 64);
        spriteSheet->addFrame(128, 0, 64, 64);
        spriteSheet->addFrame(192, 0, 64, 64);
        
        // Walk 动画帧 (Frame 4-9)
        spriteSheet->addFrame(0, 64, 64, 64);
        spriteSheet->addFrame(64, 64, 64, 64);
        spriteSheet->addFrame(128, 64, 64, 64);
        spriteSheet->addFrame(192, 64, 64, 64);
        spriteSheet->addFrame(256, 64, 64, 64);
        spriteSheet->addFrame(320, 64, 64, 64);
        
        // 方式2：使用网格方法（注释掉，因为已经手动添加）
        // spriteSheet->addFrameGrid(0, 0, 64, 64, 4, 1);    // Idle: 4 帧横向
        // spriteSheet->addFrameGrid(0, 64, 64, 64, 6, 1);   // Walk: 6 帧横向
        
        std::cout << "Total frames defined: " << spriteSheet->getFrameCount() << std::endl;
        
        // 创建渲染器
        renderer = std::make_unique<Sprite::SpriteRenderer>(spriteSheet.get());
        
        // 定义动画
        std::cout << "Creating animations..." << std::endl;
        
        // Idle 动画：帧 0-3，10 FPS
        Sprite::Animation idleAnim("idle", {0, 1, 2, 3}, 10.0f);
        idleAnim.setLooping(true);
        renderer->addAnimation(idleAnim);
        
        // Walk 动画：帧 4-9，12 FPS
        Sprite::Animation walkAnim("walk", {4, 5, 6, 7, 8, 9}, 12.0f);
        walkAnim.setLooping(true);
        renderer->addAnimation(walkAnim);
        
        // 设置初始动画
        renderer->setAnimation("idle");
        
        std::cout << "Initialization complete!" << std::endl;
        std::cout << "\nControls:" << std::endl;
        std::cout << "- Use ImGui panel to control animation" << std::endl;
        std::cout << "- Switch between Idle and Walk animations" << std::endl;
        std::cout << "- Adjust speed and toggle loop" << std::endl;
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
            if (ImGui::RadioButton("Idle", selectedAnim == 0)) {
                selectedAnim = 0;
                renderer->setAnimation("idle");
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Walk", selectedAnim == 1)) {
                selectedAnim = 1;
                renderer->setAnimation("walk");
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
                "Sprite Animation System Demo:\n\n"
                "This demo shows how to use the sprite animation system. "
                "You can switch between different animations, control playback speed, "
                "toggle looping, and pause/resume playback.\n\n"
                "Note: If you see errors, make sure you have a sprite sheet at "
                "'textures/sprite_sheet.png' with the expected layout."
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

