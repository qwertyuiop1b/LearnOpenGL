#include "SpriteSheet.h"
#include "../utils/Texture.h"
#include <stdexcept>
#include <iostream>

namespace Sprite {

SpriteSheet::SpriteSheet(const std::string& texturePath)
    : textureWidth(0)
    , textureHeight(0)
{
    // 加载纹理
    texture = std::make_unique<Texture>(texturePath);
    
    if (!texture->isValid()) {
        std::cerr << "Failed to load sprite sheet texture: " << texturePath << std::endl;
        return;
    }
    
    // 获取纹理尺寸
    textureWidth = texture->getWidth();
    textureHeight = texture->getHeight();
    
    std::cout << "Sprite sheet loaded: " << texturePath 
              << " (" << textureWidth << "x" << textureHeight << ")" << std::endl;
}

void SpriteSheet::addFrame(int x, int y, int w, int h) {
    if (!texture || !texture->isValid()) {
        std::cerr << "Cannot add frame: texture not loaded" << std::endl;
        return;
    }
    
    if (textureWidth == 0 || textureHeight == 0) {
        std::cerr << "Cannot add frame: invalid texture dimensions" << std::endl;
        return;
    }
    
    // 计算 UV 坐标（归一化到 [0, 1]）
    // 注意：OpenGL 纹理坐标原点在左下角，但图片通常是左上角
    // 我们需要翻转 Y 坐标
    Frame frame;
    frame.u0 = static_cast<float>(x) / textureWidth;
    frame.v0 = 1.0f - static_cast<float>(y + h) / textureHeight;  // 翻转 Y
    frame.u1 = static_cast<float>(x + w) / textureWidth;
    frame.v1 = 1.0f - static_cast<float>(y) / textureHeight;      // 翻转 Y
    frame.width = static_cast<float>(w);
    frame.height = static_cast<float>(h);
    
    frames.push_back(frame);
    
    std::cout << "Frame " << (frames.size() - 1) << " added: "
              << "pos(" << x << "," << y << ") "
              << "size(" << w << "x" << h << ") "
              << "UV(" << frame.u0 << "," << frame.v0 << ")-(" 
              << frame.u1 << "," << frame.v1 << ")" << std::endl;
}

void SpriteSheet::addFrameGrid(int startX, int startY,
                               int frameWidth, int frameHeight,
                               int cols, int rows, int count)
{
    int totalFrames = (count == -1) ? (cols * rows) : count;
    int frameIndex = 0;
    
    for (int row = 0; row < rows && frameIndex < totalFrames; ++row) {
        for (int col = 0; col < cols && frameIndex < totalFrames; ++col) {
            int x = startX + col * frameWidth;
            int y = startY + row * frameHeight;
            addFrame(x, y, frameWidth, frameHeight);
            ++frameIndex;
        }
    }
    
    std::cout << "Added " << totalFrames << " frames in " 
              << cols << "x" << rows << " grid" << std::endl;
}

const Frame& SpriteSheet::getFrame(size_t index) const {
    if (index >= frames.size()) {
        throw std::out_of_range("Frame index out of range: " + 
                                std::to_string(index) + 
                                " (max: " + std::to_string(frames.size()) + ")");
    }
    return frames[index];
}

void SpriteSheet::bind(GLuint unit) const {
    if (texture && texture->isValid()) {
        texture->bind(unit);
    }
}

void SpriteSheet::unbind() const {
    if (texture && texture->isValid()) {
        texture->unbind();
    }
}

bool SpriteSheet::isValid() const {
    return texture && texture->isValid() && !frames.empty();
}

} // namespace Sprite

