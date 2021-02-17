#include <glad/glad.h>

#include "VertexBuffer.hpp"

VertexBuffer::VertexBuffer(unsigned int type) {
    Create(type);
}

VertexBuffer::~VertexBuffer() {
    Destroy();
}

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
    : m_handle(other.m_handle)
    , m_type(other.m_type) {
    other.m_handle = 0;
    other.m_type = 0;
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept {
    if (this != &other) {
        Destroy();

        m_handle = other.m_handle;
        m_type = other.m_type;

        other.m_handle = 0;
        other.m_type = 0;
    }

    return *this;
}

void VertexBuffer::Create(unsigned int type) {
    m_type = type;
    glGenBuffers(1, &m_handle);
}

void VertexBuffer::Destroy() {
    glDeleteBuffers(1, &m_handle);
    m_handle = 0;
}

auto VertexBuffer::IsCreated() const -> bool {
    return m_handle != 0;
}

void VertexBuffer::Buffer(const void* buffer, unsigned int size, unsigned int usage) const {
    Bind();
    glBufferData(m_type, size, buffer, usage);
}

void VertexBuffer::Bind() const {
    glBindBuffer(m_type, m_handle);
}

auto VertexBuffer::GetHandle() const -> unsigned int {
    return m_handle;
}

auto VertexBuffer::GetType() const -> unsigned int {
    return m_type;
}
