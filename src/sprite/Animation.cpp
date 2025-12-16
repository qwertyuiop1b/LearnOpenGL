#include "Animation.h"
#include <iostream>

namespace Sprite {

Animation::Animation(const std::string& name,
                     const std::vector<int>& frames,
                     float fps)
    : name(name)
    , frameIndices(frames)
    , frameDuration(1.0f / fps)
    , looping(true)
    , speed(1.0f)
    , paused(false)
    , currentFrameIndex(0)
    , elapsedTime(0.0f)
    , finished(false)
{
    if (frameIndices.empty()) {
        std::cerr << "Warning: Animation '" << name 
                  << "' created with no frames!" << std::endl;
    }
}

void Animation::update(float deltaTime) {
    // 如果暂停或已完成，不更新
    if (paused || finished) {
        return;
    }
    
    // 如果没有帧，不更新
    if (frameIndices.empty()) {
        return;
    }
    
    // 累加时间（考虑速度倍率）
    elapsedTime += deltaTime * speed;
    
    // 检查是否需要切换到下一帧
    while (elapsedTime >= frameDuration) {
        elapsedTime -= frameDuration;
        currentFrameIndex++;
        
        // 检查是否到达序列末尾
        if (currentFrameIndex >= frameIndices.size()) {
            if (looping) {
                // 循环：回到第一帧
                currentFrameIndex = 0;
            } else {
                // 不循环：停在最后一帧
                currentFrameIndex = frameIndices.size() - 1;
                finished = true;
                elapsedTime = 0.0f;
                break;
            }
        }
    }
}

void Animation::play() {
    // 如果已完成，重置
    if (finished) {
        reset();
    }
    
    paused = false;
}

void Animation::pause() {
    paused = true;
}

void Animation::reset() {
    currentFrameIndex = 0;
    elapsedTime = 0.0f;
    finished = false;
    paused = false;
}

void Animation::setSpeed(float multiplier) {
    if (multiplier < 0.0f) {
        std::cerr << "Warning: Animation speed cannot be negative, clamping to 0.0f" << std::endl;
        multiplier = 0.0f;
    }
    speed = multiplier;
}

void Animation::setLooping(bool loop) {
    looping = loop;
    
    // 如果设置为循环且动画已完成，重置
    if (looping && finished) {
        finished = false;
    }
}

void Animation::setFPS(float fps) {
    if (fps <= 0.0f) {
        std::cerr << "Warning: FPS must be positive, using 10.0f" << std::endl;
        fps = 10.0f;
    }
    frameDuration = 1.0f / fps;
}

int Animation::getCurrentFrame() const {
    if (frameIndices.empty()) {
        return -1;
    }
    
    // 返回当前帧在 SpriteSheet 中的索引
    return frameIndices[currentFrameIndex];
}

} // namespace Sprite

