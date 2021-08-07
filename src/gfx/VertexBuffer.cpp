#include <glad/glad.h>

#include "VertexBuffer.hpp"

VertexBuffer::VertexBuffer(u32 type) {
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

void VertexBuffer::create(u32 type) {
    m_type = type;
    glGenBuffers(1, &m_handle);
}

void VertexBuffer::destroy() {
    glDeleteBuffers(1, &m_handle);
    m_handle = 0;
}

void VertexBuffer::buffer(const void* buffer, u32 size, u32 usage) const {
    bind();
    glBufferData(m_type, i32(size), buffer, usage);
}

void VertexBuffer::bind() const {
    glBindBuffer(m_type, m_handle);
}

auto VertexBuffer::is_created() const -> bool {
    return m_handle != 0;
}

auto VertexBuffer::get_handle() const -> u32 {
    return m_handle;
}

auto VertexBuffer::get_type() const -> u32 {
    return m_type;
}
