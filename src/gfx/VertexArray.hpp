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

    void Create();
    void Destroy();

    auto IsCreated() const -> bool;

    void Bind() const;
    void Attributes(const VertexBuffer& buffer, const VertexLayout& layout) const;

    auto GetHandle() const -> unsigned int;

private:
    unsigned int m_handle { 0 };
};
