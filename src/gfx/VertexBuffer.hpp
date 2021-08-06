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

    void create(unsigned int type);
    void destroy();

    auto is_created() const -> bool;

    void bind() const;
    void buffer(const void* buffer, unsigned int size, unsigned int usage = GL_STATIC_DRAW) const;

    auto get_handle() const -> unsigned int;
    auto get_type() const -> unsigned int;

private:
    unsigned int m_handle { 0 };
    unsigned int m_type { 0 };
};