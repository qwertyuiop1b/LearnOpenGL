#ifndef OPENGL_SPRITE_TYPES_H
#define OPENGL_SPRITE_TYPES_H

namespace Sprite {

/**
 * Frame 结构
 * 
 * 表示精灵图中的一帧
 * 存储该帧在纹理中的 UV 坐标和尺寸信息
 */
struct Frame {
    // UV 坐标（归一化到 [0, 1]）
    float u0, v0;  // 左下角 UV 坐标
    float u1, v1;  // 右上角 UV 坐标
    
    // 原始像素尺寸（可选，用于调试和显示）
    float width;   // 帧宽度（像素）
    float height;  // 帧高度（像素）
    
    Frame() 
        : u0(0.0f), v0(0.0f)
        , u1(1.0f), v1(1.0f)
        , width(0.0f), height(0.0f)
    {}
    
    Frame(float u0, float v0, float u1, float v1, float w = 0.0f, float h = 0.0f)
        : u0(u0), v0(v0)
        , u1(u1), v1(v1)
        , width(w), height(h)
    {}
};

} // namespace Sprite

#endif // OPENGL_SPRITE_TYPES_H

