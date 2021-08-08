#pragma once

#include "vertex_buffer.hpp"
#include "vertex_layout.hpp"

class VertexArray {
public:
    VertexArray() = default;
    VertexArray(const VertexBuffer& buffer, const VertexLayout& layout);

    ~VertexArray();

    VertexArray(const VertexArray&) = delete;
    VertexArray& operator=(const VertexArray&) = delete;

    VertexArray(VertexArray&& other) noexcept;
    VertexArray& operator=(VertexArray&& other) noexcept;

    void create();
    void destroy();

    void bind() const;

    void attributes(const VertexBuffer& buffer, const VertexLayout& layout) const;

    [[nodiscard]]
    auto is_created() const -> bool;

    [[nodiscard]]
    auto get_handle() const -> u32;


private:
    u32 m_handle { 0 };
};
