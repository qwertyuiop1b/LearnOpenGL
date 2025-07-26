#ifndef OPENGL_UTILS_VERTEX_ARRAY_H
#define OPENGL_UTILS_VERTEX_ARRAY_H

#include "VertexBuffer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

enum class AttributeType {
    Float   = GL_FLOAT,
    Int     = GL_INT,
    UInt    = GL_UNSIGNED_INT
};

struct VertexAttribute {
    GLuint      index;
    GLint       size;
    AttributeType type;
    bool        normalized;
    GLsizei     stride;
    const void* offset;
};

class VertexArray {
private:
    GLuint mId;
    bool mIndexBufferBound;
    std::vector<GLuint> mVboIds;
public:
    VertexArray();
    ~VertexArray();

    VertexArray(const VertexArray&) = delete;
    VertexArray& operator=(const VertexArray&) = delete;

    VertexArray(VertexArray&& other) noexcept;
    VertexArray& operator=(VertexArray&& other) noexcept;

    void bind() const;
    void unbind() const;

    VertexArray& addVertexBuffer(const VertexBuffer& vbo, const std::vector<VertexAttribute>& attributes);

    VertexArray& setIndexBuffer(const VertexBuffer& ibo);

};

#endif