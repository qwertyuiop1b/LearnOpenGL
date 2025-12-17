#ifndef OPENGL_SPRITE_SHEET_H
#define OPENGL_SPRITE_SHEET_H

#include <string>
#include <vector>
#include <memory>
#include <glad/glad.h>
#include "SpriteTypes.h"
#include "utils/Texture.h"

namespace Sprite {

/**
 * SpriteSheet 类
 * 
 * 管理精灵图纹理和帧定义
 * 支持不规则布局（手动指定每帧的位置和大小）
 * 
 * 使用示例：
 *   SpriteSheet sheet("character.png");
 *   sheet.addFrame(0, 0, 64, 64);     // Frame 0
 *   sheet.addFrame(64, 0, 64, 64);    // Frame 1
 *   sheet.addFrame(128, 0, 64, 64);   // Frame 2
 */
class SpriteSheet {
private:
    std::unique_ptr<Texture> texture;  // 精灵图纹理
    std::vector<Frame> frames;          // 帧定义列表
    int textureWidth;                   // 纹理宽度（像素）
    int textureHeight;                  // 纹理高度（像素）

public:
    /**
     * 构造函数
     * 
     * @param texturePath 精灵图纹理文件路径
     * 
     * 加载纹理并初始化
     * 注意：需要在 OpenGL 上下文创建后调用
     */
    explicit SpriteSheet(const std::string& texturePath);
    
    /**
     * 析构函数
     * 
     * 自动释放纹理资源（通过 Texture2 的 RAII）
     */
    ~SpriteSheet() = default;
    
    // 禁用拷贝（纹理资源不应拷贝）
    SpriteSheet(const SpriteSheet&) = delete;
    SpriteSheet& operator=(const SpriteSheet&) = delete;
    
    // 允许移动
    SpriteSheet(SpriteSheet&&) noexcept = default;
    SpriteSheet& operator=(SpriteSheet&&) noexcept = default;
    
    /**
     * 添加帧定义
     * 
     * @param x 帧在纹理中的 X 坐标（像素，左上角为原点）
     * @param y 帧在纹理中的 Y 坐标（像素，左上角为原点）
     * @param w 帧宽度（像素）
     * @param h 帧高度（像素）
     * 
     * 自动计算 UV 坐标并存储
     * 
     * 示例：
     *   sheet.addFrame(0, 0, 64, 64);     // 添加第一帧
     *   sheet.addFrame(64, 0, 64, 64);    // 添加第二帧（右侧）
     */
    void addFrame(int x, int y, int w, int h);
    
    /**
     * 添加网格布局的帧
     * 
     * @param startX 起始 X 坐标（像素）
     * @param startY 起始 Y 坐标（像素）
     * @param frameWidth 每帧宽度（像素）
     * @param frameHeight 每帧高度（像素）
     * @param cols 列数
     * @param rows 行数
     * @param count 要添加的帧数（-1 表示全部）
     * 
     * 便捷方法，用于规则网格布局
     * 
     * 示例：
     *   sheet.addFrameGrid(0, 0, 64, 64, 8, 2);  // 8x2 网格，每帧 64x64
     */
    void addFrameGrid(int startX, int startY, 
                      int frameWidth, int frameHeight,
                      int cols, int rows, int count = -1);
    
    /**
     * 获取指定帧
     * 
     * @param index 帧索引（0 开始）
     * @return 帧定义的常量引用
     * 
     * 注意：如果索引越界，会抛出异常
     */
    const Frame& getFrame(size_t index) const;
    
    /**
     * 获取帧总数
     * 
     * @return 已定义的帧数量
     */
    size_t getFrameCount() const { return frames.size(); }
    
    /**
     * 绑定纹理到指定纹理单元
     * 
     * @param unit 纹理单元索引（默认 0）
     */
    void bind(GLuint unit = 0) const;
    
    /**
     * 解绑纹理
     */
    void unbind() const;
    
    /**
     * 检查是否有效
     * 
     * @return true 如果纹理已加载且至少有一帧
     */
    bool isValid() const;
    
    /**
     * 获取纹理尺寸
     */
    int getTextureWidth() const { return textureWidth; }
    int getTextureHeight() const { return textureHeight; }
};

} // namespace Sprite

#endif // OPENGL_SPRITE_SHEET_H

