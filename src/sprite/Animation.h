#ifndef OPENGL_SPRITE_ANIMATION_H
#define OPENGL_SPRITE_ANIMATION_H

#include <string>
#include <vector>

namespace Sprite {

/**
 * Animation 类
 * 
 * 管理动画序列的播放
 * 支持循环、速度控制、暂停等功能
 * 
 * 使用示例：
 *   Animation idle("idle", {0, 1, 2, 3}, 10.0f);
 *   idle.setLooping(true);
 *   idle.play();
 *   
 *   // 每帧更新
 *   idle.update(deltaTime);
 *   int currentFrame = idle.getCurrentFrame();
 */
class Animation {
private:
    std::string name;                    // 动画名称
    std::vector<int> frameIndices;       // 帧索引序列
    float frameDuration;                 // 每帧持续时间（秒）
    bool looping;                        // 是否循环播放
    float speed;                         // 播放速度倍率（1.0 = 正常速度）
    bool paused;                         // 是否暂停
    
    // 运行时状态
    size_t currentFrameIndex;            // 当前帧在序列中的索引
    float elapsedTime;                   // 当前帧已播放时间
    bool finished;                       // 是否播放完成（非循环动画）
    
public:
    /**
     * 构造函数
     * 
     * @param name 动画名称（用于识别和切换）
     * @param frames 帧索引序列
     * @param fps 帧率（每秒帧数）
     * 
     * 示例：
     *   Animation walk("walk", {4, 5, 6, 7, 8, 9}, 12.0f);
     */
    Animation(const std::string& name, 
              const std::vector<int>& frames,
              float fps = 10.0f);
    
    /**
     * 更新动画
     * 
     * @param deltaTime 时间增量（秒）
     * 
     * 每帧调用，更新动画状态
     * 如果暂停，不会更新
     */
    void update(float deltaTime);
    
    /**
     * 播放/继续播放
     * 
     * 如果动画已完成，会重置到开始
     */
    void play();
    
    /**
     * 暂停
     */
    void pause();
    
    /**
     * 重置到第一帧
     */
    void reset();
    
    /**
     * 设置播放速度
     * 
     * @param multiplier 速度倍率
     *   - 1.0 = 正常速度
     *   - 0.5 = 半速
     *   - 2.0 = 两倍速
     */
    void setSpeed(float multiplier);
    
    /**
     * 设置是否循环
     * 
     * @param loop true = 循环，false = 播放一次后停止
     */
    void setLooping(bool loop);
    
    /**
     * 设置帧率
     * 
     * @param fps 每秒帧数
     */
    void setFPS(float fps);
    
    /**
     * 获取当前帧索引（在 SpriteSheet 中的索引）
     * 
     * @return 当前帧的索引
     */
    int getCurrentFrame() const;
    
    /**
     * 获取当前帧在序列中的索引
     * 
     * @return 当前帧在 frameIndices 中的索引
     */
    size_t getCurrentFrameIndex() const { return currentFrameIndex; }
    
    /**
     * 检查是否播放完成
     * 
     * @return true 如果动画播放完成且不循环
     */
    bool isFinished() const { return finished; }
    
    /**
     * 检查是否暂停
     * 
     * @return true 如果动画暂停
     */
    bool isPaused() const { return paused; }
    
    /**
     * 获取动画名称
     */
    const std::string& getName() const { return name; }
    
    /**
     * 获取帧数量
     */
    size_t getFrameCount() const { return frameIndices.size(); }
    
    /**
     * 获取播放速度
     */
    float getSpeed() const { return speed; }
    
    /**
     * 获取是否循环
     */
    bool isLooping() const { return looping; }
    
    /**
     * 获取帧率
     */
    float getFPS() const { return (frameDuration > 0.0f) ? (1.0f / frameDuration) : 0.0f; }
};

} // namespace Sprite

#endif // OPENGL_SPRITE_ANIMATION_H

