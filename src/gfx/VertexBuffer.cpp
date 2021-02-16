#include <glad/glad.h>

#include "VertexBuffer.hpp"

VertexBuffer::VertexBuffer(unsigned int type) :
    m_type(type) {
    glGenBuffers(1, &m_handle);
}

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept :
    m_handle(other.m_handle), m_type(other.m_type) {
    other.m_handle = 0;
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &m_handle);
}

void VertexBuffer::Generate(const void* buffer, unsigned int size, unsigned int usage) const {
    Bind();
    glBufferData(m_type, size, buffer, usage);
}

void VertexBuffer::Bind() const {
    glBindBuffer(m_type, m_handle);
}

unsigned int VertexBuffer::GetHandle() const {
    return m_handle;
}

unsigned int VertexBuffer::GetType() const {
    return m_type;
}
