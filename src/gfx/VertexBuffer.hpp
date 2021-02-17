#pragma once

#include <glad/glad.h>

class VertexBuffer {
public:
    VertexBuffer() = default;
    explicit VertexBuffer(unsigned int type);

    ~VertexBuffer();

    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;

    VertexBuffer(VertexBuffer&& other) noexcept;
    VertexBuffer& operator=(VertexBuffer&& other) noexcept;

    void Create(unsigned int type);
    void Destroy();

    auto IsCreated() const -> bool;

    void Bind() const;
    void Buffer(const void* buffer, unsigned int size, unsigned int usage = GL_STATIC_DRAW) const;

    auto GetHandle() const -> unsigned int;
    auto GetType() const -> unsigned int;

private:
    unsigned int m_handle { 0 };
    unsigned int m_type { 0 };
};