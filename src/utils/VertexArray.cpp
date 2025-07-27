#include "utils/VertexArray.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "utils/Application.h"
#include <utility>
#include <vector>
#include <iostream>



VertexArray::VertexArray(): mId(0), mIndexBufferBound(false), mVboIds() {
}

VertexArray::~VertexArray() {
    if (mId) {
        glDeleteVertexArrays(1, &mId);
    }
}

VertexArray::VertexArray(VertexArray&& other) noexcept {
    mId = other.mId;
    mVboIds = std::move(other.mVboIds);
    mIndexBufferBound = other.mIndexBufferBound;
    other.mId = 0;
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept {
    if (this != &other) {
        if (mId) glDeleteVertexArrays(1, &mId);
        mId = other.mId;
        mVboIds = std::move(other.mVboIds);
        mIndexBufferBound = other.mIndexBufferBound;
        other.mId = 0;
    }
    return *this;
}

void VertexArray::create() {
    glGenVertexArrays(1, &mId);
}

void VertexArray::bind() const {
    glBindVertexArray(mId);
}

void VertexArray::unbind() const {
    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    if (mIndexBufferBound) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}

VertexArray& VertexArray::addVertexBuffer(const VertexBuffer& vbo, const std::vector<VertexAttribute>& attributes) {
    bind();
    vbo.bind();

    for (const auto& attr: attributes) {
        glEnableVertexAttribArray(attr.index);

        if (attr.type == AttributeType::Int || attr.type == AttributeType::UInt) {
            glVertexAttribIPointer(
                attr.index,
                attr.size,
                static_cast<GLenum>(attr.type),
                attr.stride,
                attr.offset
            );
        } else {
            glVertexAttribPointer(
                attr.index,
                attr.size,
                static_cast<GLenum>(attr.type),
                attr.normalized ? GL_TRUE : GL_FALSE,
                attr.stride,
                attr.offset
            );
        }
    }
    mVboIds.push_back(vbo.id()); // vbo 引用
    return *this;
}

VertexArray& VertexArray::setIndexBuffer(const VertexBuffer& ibo) {
    bind();
    ibo.bind();
    mIndexBufferBound = true;
    return *this;
}