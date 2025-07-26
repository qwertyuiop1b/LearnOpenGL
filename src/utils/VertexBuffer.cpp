#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(GLenum targetType): mId(0), mTargetType(targetType) {
    glGenBuffers(1, &mId);
}

VertexBuffer::~VertexBuffer() {
    if (mId != 0) {
        glDeleteBuffers(1, &mId);
        mId = 0;
    }
}

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept : mId(other.mId) {
    other.mId = 0;
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept {
    if (this != &other) {
        if (mId != 0) {
            glDeleteBuffers(1, &mId);
        }
        mId = other.mId;
        other.mId = 0;
    }
    return *this;
}

void VertexBuffer::bind() const {
    glBindBuffer(mTargetType, mId);
}

void VertexBuffer::unbind() const {
    glBindBuffer(mTargetType, 0);
}