#include "FPS.h"
#include <iostream>
#include <thread>


FPS::FPS(uint32_t targetFPS)
    : targetFrameRate(targetFPS)
    , targetFrameTime(1.0f / targetFPS)
    , deltaTime(1.0f / targetFPS)
    , smoothFactor(0.9f)
    , lastFrameTime(std::chrono::high_resolution_clock::now()) {
}


void FPS::update() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> elapsed = currentTime - lastFrameTime;
    
    // 帧率限制：如果时间不足目标帧时间，则 sleep 延迟
    if (elapsed.count() < targetFrameTime) {
        std::chrono::duration<float> sleepDuration(targetFrameTime - elapsed.count());
        std::this_thread::sleep_for(sleepDuration);
        
        // 重新获取当前时间
        currentTime = std::chrono::high_resolution_clock::now();
        elapsed = currentTime - lastFrameTime;
    }
    
    // 使用指数移动平均平滑 deltaTime
    float currentFrameTime = elapsed.count();
    deltaTime = smoothFactor * deltaTime + (1.0f - smoothFactor) * currentFrameTime;
    // deltaTime = elapsed.count();
    
    // 更新上一帧时间
    lastFrameTime = currentTime;

}

void FPS::setTargetFrameRate(uint32_t fps) {
    targetFrameRate = fps;
    targetFrameTime = 1.0f / fps;
}

float FPS::getDeltaTime() const {
    return deltaTime;
}



