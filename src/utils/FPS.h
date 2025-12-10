#ifndef OPENGL_UTILS_FPS_H
#define OPENGL_UTILS_FPS_H

#include <chrono>
#include <cstdint>

class FPS {
public:
    FPS(uint32_t targetFPS = 60);
    ~FPS() = default;
    void update();
    void setTargetFrameRate(uint32_t fps);
    float getDeltaTime() const;
private:
    // 帧率限制相关
    uint32_t targetFrameRate;
    float targetFrameTime;
    
    // deltaTime 相关
    float deltaTime;
    float smoothFactor;
    std::chrono::high_resolution_clock::time_point lastFrameTime;

};

#endif // OPENGL_UTILS_FPS_H