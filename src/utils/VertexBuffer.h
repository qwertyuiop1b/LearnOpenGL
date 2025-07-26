#ifndef OPENGL_VERTEX_BUFFER_H
#define OPENGL_VERTEX_BUFFER_H

#include "utils/Application.h"
#include <cstddef>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

enum class BufferUsage {
    StaticDraw = GL_STATIC_DRAW,
    DynamicDraw = GL_DYNAMIC_DRAW,
    StreamDraw = GL_STREAM_DRAW
};




class VertexBuffer {
private:
    unsigned int mId;
    GLenum mTargetType = GL_ARRAY_BUFFER;
public:
    VertexBuffer(GLenum targetType = GL_ARRAY_BUFFER);
    ~VertexBuffer();

    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;

    VertexBuffer(VertexBuffer&& other) noexcept;
    VertexBuffer& operator=(VertexBuffer&& other) noexcept;

    void bind() const;

    void unbind() const;

    template<typename T>
    void upload(const std::vector<T>& data, BufferUsage usage = BufferUsage::StaticDraw) {
        bind();
        glBufferData(mTargetType, data.size() * sizeof(T), data.data(), static_cast<GLenum>(usage));
    }

    template<typename T>
    void upload(const T* data, size_t count, BufferUsage usage = BufferUsage::StaticDraw) {
        bind();
        glBufferData(mTargetType, count * sizeof(T), data, static_cast<GLenum>(usage));
    }

    GLuint id() const { return mId; }
};
#endif