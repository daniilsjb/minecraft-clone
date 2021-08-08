#pragma once

#include <glad/glad.h>

#include "../common/types.hpp"

class VertexBuffer {
public:
    VertexBuffer() = default;
    explicit VertexBuffer(u32 type);

    ~VertexBuffer();

    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;

    VertexBuffer(VertexBuffer&& other) noexcept;
    VertexBuffer& operator=(VertexBuffer&& other) noexcept;

    void create(u32 type);
    void destroy();

    void bind() const;
    void buffer(const void* buffer, u32 size, u32 usage = GL_STATIC_DRAW) const;

    [[nodiscard]]
    auto is_created() const -> bool;

    [[nodiscard]]
    auto get_handle() const -> u32;

    [[nodiscard]]
    auto get_type() const -> u32;

private:
    u32 m_handle { 0 };
    u32 m_type { 0 };
};