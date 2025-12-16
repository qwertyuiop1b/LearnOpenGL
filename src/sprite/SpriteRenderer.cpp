#include "SpriteRenderer.h"
#include "SpriteSheet.h"
#include "Animation.h"
#include "../utils/Shader.h"
#include "../utils/VertexArray.h"
#include "../utils/VertexBuffer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace Sprite {

SpriteRenderer::SpriteRenderer(SpriteSheet* sheet)
    : spriteSheet(sheet)
    , currentAnimation(nullptr)
{
    if (!spriteSheet) {
        std::cerr << "Warning: SpriteRenderer created with null SpriteSheet!" << std::endl;
        return;
    }
    
    // 创建着色器
    shader = std::make_unique<Shader>(
        "shaders/sprite/sprite.vert",
        "shaders/sprite/sprite.frag"
    );
    
    // 初始化渲染数据
    initRenderData();
    
    std::cout << "SpriteRenderer created" << std::endl;
}

void SpriteRenderer::initRenderData() {
    // 创建 VAO
    vao = std::make_unique<VertexArray>();
    vao->bind();
    
    // 顶点数据：位置 (x, y) 和纹理坐标 (u, v)
    // 初始 UV 覆盖整个纹理 [0, 1]
    float vertices[] = {
        // 位置        // UV
        0.0f, 1.0f,   0.0f, 1.0f,  // 左上
        1.0f, 0.0f,   1.0f, 0.0f,  // 右下
        0.0f, 0.0f,   0.0f, 0.0f,  // 左下
        
        0.0f, 1.0f,   0.0f, 1.0f,  // 左上
        1.0f, 1.0f,   1.0f, 1.0f,  // 右上
        1.0f, 0.0f,   1.0f, 0.0f   // 右下
    };
    
    // 创建 VBO 并上传数据
    auto vbo = std::make_unique<VertexBuffer>();
    vbo->upload(vertices, 24, BufferUsage::DynamicDraw);
    
    // 设置顶点属性
    // 位置属性 (location = 0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    
    // UV 属性 (location = 1)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    
    // 解绑
    vao->unbind();
    
    std::cout << "Sprite render data initialized" << std::endl;
}

void SpriteRenderer::updateVertexUV(const glm::vec4& uv) {
    // uv = (u0, v0, u1, v1)
    float vertices[] = {
        // 位置        // UV
        0.0f, 1.0f,   uv.x, uv.w,  // 左上
        1.0f, 0.0f,   uv.z, uv.y,  // 右下
        0.0f, 0.0f,   uv.x, uv.y,  // 左下
        
        0.0f, 1.0f,   uv.x, uv.w,  // 左上
        1.0f, 1.0f,   uv.z, uv.w,  // 右上
        1.0f, 0.0f,   uv.z, uv.y   // 右下
    };
    
    // 更新 VBO
    vao->bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    vao->unbind();
}

void SpriteRenderer::addAnimation(const Animation& anim) {
    const std::string& name = anim.getName();
    animations.insert_or_assign(name, anim);
    std::cout << "Animation '" << name << "' added (" 
              << anim.getFrameCount() << " frames)" << std::endl;
}

bool SpriteRenderer::setAnimation(const std::string& name) {
    auto it = animations.find(name);
    if (it == animations.end()) {
        std::cerr << "Animation '" << name << "' not found!" << std::endl;
        return false;
    }
    
    // 切换动画并重置
    currentAnimation = &it->second;
    currentAnimation->reset();
    currentAnimation->play();
    
    std::cout << "Switched to animation: " << name << std::endl;
    return true;
}

void SpriteRenderer::update(float deltaTime) {
    if (currentAnimation) {
        currentAnimation->update(deltaTime);
    }
}

void SpriteRenderer::render(const glm::vec2& position,
                            const glm::vec2& size,
                            float rotation,
                            const glm::vec3& color)
{
    if (!spriteSheet || !spriteSheet->isValid()) {
        std::cerr << "Cannot render: invalid sprite sheet!" << std::endl;
        return;
    }
    
    if (!currentAnimation) {
        std::cerr << "Cannot render: no current animation!" << std::endl;
        return;
    }
    
    // 获取当前帧
    int frameIndex = currentAnimation->getCurrentFrame();
    if (frameIndex < 0 || frameIndex >= static_cast<int>(spriteSheet->getFrameCount())) {
        std::cerr << "Invalid frame index: " << frameIndex << std::endl;
        return;
    }
    
    const Frame& frame = spriteSheet->getFrame(frameIndex);
    
    // 更新 UV 坐标
    glm::vec4 uv(frame.u0, frame.v0, frame.u1, frame.v1);
    updateVertexUV(uv);
    
    // 使用着色器
    shader->use();
    
    // 构建模型矩阵（变换）
    glm::mat4 model = glm::mat4(1.0f);
    
    // 平移
    model = glm::translate(model, glm::vec3(position, 0.0f));
    
    // 旋转（围绕中心旋转）
    if (rotation != 0.0f) {
        model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
        model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
    }
    
    // 缩放
    model = glm::scale(model, glm::vec3(size, 1.0f));
    
    // 设置投影矩阵（正交投影，适用于 2D）
    glm::mat4 projection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -1.0f, 1.0f);
    shader->setMatrix4("uProjection", projection);
    
    // 设置 uniform
    shader->setMatrix4("uModel", model);
    shader->setFloat3("uColor", color);
    
    // 绑定纹理
    spriteSheet->bind(0);
    shader->setInt("uTexture", 0);
    
    // 渲染
    vao->bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    vao->unbind();
}

void SpriteRenderer::play() {
    if (currentAnimation) {
        currentAnimation->play();
    }
}

void SpriteRenderer::pause() {
    if (currentAnimation) {
        currentAnimation->pause();
    }
}

Animation* SpriteRenderer::getCurrentAnimation() {
    return currentAnimation;
}

const Animation* SpriteRenderer::getCurrentAnimation() const {
    return currentAnimation;
}

} // namespace Sprite

