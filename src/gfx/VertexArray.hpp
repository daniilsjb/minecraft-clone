#pragma once

#include "VertexBuffer.hpp"
#include "VertexLayout.hpp"

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

    auto is_created() const -> bool;

    void bind() const;
    void attributes(const VertexBuffer& buffer, const VertexLayout& layout) const;

    auto get_handle() const -> unsigned int;

private:
    unsigned int m_handle { 0 };
};
