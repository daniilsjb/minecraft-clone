#include <glad/glad.h>

#include "VertexBuffer.hpp"

VertexBuffer::VertexBuffer(unsigned int type) {
    create(type);
}

VertexBuffer::~VertexBuffer() {
    destroy();
}

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
    : m_handle(other.m_handle)
    , m_type(other.m_type) {
    other.m_handle = 0;
    other.m_type = 0;
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept {
    if (this != &other) {
        destroy();

        m_handle = other.m_handle;
        m_type = other.m_type;

        other.m_handle = 0;
        other.m_type = 0;
    }

    return *this;
}

void VertexBuffer::create(unsigned int type) {
    m_type = type;
    glGenBuffers(1, &m_handle);
}

void VertexBuffer::destroy() {
    glDeleteBuffers(1, &m_handle);
    m_handle = 0;
}

auto VertexBuffer::is_created() const -> bool {
    return m_handle != 0;
}

void VertexBuffer::buffer(const void* buffer, unsigned int size, unsigned int usage) const {
    bind();
    glBufferData(m_type, size, buffer, usage);
}

void VertexBuffer::bind() const {
    glBindBuffer(m_type, m_handle);
}

auto VertexBuffer::get_handle() const -> unsigned int {
    return m_handle;
}

auto VertexBuffer::get_type() const -> unsigned int {
    return m_type;
}
