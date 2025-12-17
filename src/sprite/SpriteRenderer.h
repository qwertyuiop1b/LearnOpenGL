#ifndef OPENGL_SPRITE_RENDERER_H
#define OPENGL_SPRITE_RENDERER_H

#include <memory>
#include <map>
#include <string>
#include <glm/glm.hpp>
#include "../utils/Shader.h"
#include "../utils/VertexArray.h"
#include "../utils/VertexBuffer.h"

namespace Sprite {

// 前向声明
class SpriteSheet;
class Animation;

/**
 * SpriteRenderer 类
 * 
 * 渲染 sprite 动画
 * 管理多个动画并支持切换
 * 支持位置、缩放、旋转变换
 * 
 * 使用示例：
 *   SpriteRenderer renderer(spriteSheet);
 *   renderer.addAnimation(Animation("idle", {0,1,2,3}, 10.0f));
 *   renderer.addAnimation(Animation("walk", {4,5,6,7}, 12.0f));
 *   renderer.setAnimation("idle");
 *   
 *   // 每帧
 *   renderer.update(deltaTime);
 *   renderer.render(position, size);
 */
class SpriteRenderer {
private:
    std::unique_ptr<Shader> shader;              // 精灵着色器
    std::unique_ptr<VertexArray> vao;            // VAO
    std::unique_ptr<VertexBuffer> vbo;           // VBO（需要保持引用）
    SpriteSheet* spriteSheet;                    // 精灵图（不拥有所有权）
    std::map<std::string, Animation> animations; // 动画集合
    Animation* currentAnimation;                 // 当前动画（指向 animations 中的元素）
    
    // 内部方法
    void initRenderData();                       // 初始化渲染数据（VAO/VBO）
    void updateVertexUV(const glm::vec4& uv);    // 更新顶点 UV 坐标
    
public:
    /**
     * 构造函数
     * 
     * @param sheet 精灵图指针（不转移所有权）
     * 
     * 注意：sheet 必须在 SpriteRenderer 的生命周期内保持有效
     */
    explicit SpriteRenderer(SpriteSheet* sheet);
    
    /**
     * 析构函数
     */
    ~SpriteRenderer() = default;
    
    // 禁用拷贝
    SpriteRenderer(const SpriteRenderer&) = delete;
    SpriteRenderer& operator=(const SpriteRenderer&) = delete;
    
    // 允许移动
    SpriteRenderer(SpriteRenderer&&) noexcept = default;
    SpriteRenderer& operator=(SpriteRenderer&&) noexcept = default;
    
    /**
     * 添加动画
     * 
     * @param anim 动画对象（会被拷贝）
     * 
     * 如果已存在同名动画，会被覆盖
     */
    void addAnimation(const Animation& anim);
    
    /**
     * 设置当前动画
     * 
     * @param name 动画名称
     * @return true 如果切换成功
     * 
     * 切换动画时，新动画会从第一帧开始播放
     */
    bool setAnimation(const std::string& name);
    
    /**
     * 更新动画
     * 
     * @param deltaTime 时间增量（秒）
     * 
     * 每帧调用，更新当前动画状态
     */
    void update(float deltaTime);
    
    /**
     * 渲染精灵
     * 
     * @param position 世界坐标位置（x, y）
     * @param size 精灵尺寸（width, height）
     * @param rotation 旋转角度（弧度）
     * @param color 颜色调制（默认白色）
     * 
     * 渲染当前动画的当前帧
     */
    void render(const glm::vec2& position,
                const glm::vec2& size,
                float rotation = 0.0f,
                const glm::vec3& color = glm::vec3(1.0f));
    
    /**
     * 播放当前动画
     */
    void play();
    
    /**
     * 暂停当前动画
     */
    void pause();
    
    /**
     * 获取当前动画
     * 
     * @return 当前动画指针，如果没有则返回 nullptr
     */
    Animation* getCurrentAnimation();
    const Animation* getCurrentAnimation() const;
    
    /**
     * 检查是否有动画
     * 
     * @return true 如果至少有一个动画
     */
    bool hasAnimations() const { return !animations.empty(); }
    
    /**
     * 获取动画数量
     */
    size_t getAnimationCount() const { return animations.size(); }
    
    /**
     * 获取精灵图
     */
    SpriteSheet* getSpriteSheet() { return spriteSheet; }
    const SpriteSheet* getSpriteSheet() const { return spriteSheet; }
};

} // namespace Sprite

#endif // OPENGL_SPRITE_RENDERER_H

