#pragma once

#include "../general/Mixins.hpp"
#include "VertexBuffer.hpp"
#include "VertexLayout.hpp"

class VertexArray : private NonCopyable {
public:
    VertexArray();
    VertexArray(VertexArray&& other) noexcept;
    ~VertexArray();

    void SetAttributes(const VertexBuffer& buffer, const VertexLayout& layout) const;
    void Bind() const;

    unsigned int GetHandle() const;

private:
    unsigned int m_handle { 0 };
};
