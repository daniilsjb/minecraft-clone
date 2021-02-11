#pragma once

#include <glad/glad.h>

#include <vector>

#include "VertexLayout.hpp"
#include "VertexBuffer.hpp"

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void Bind() const;
    static void Unbind();

    void AddBuffer(const VertexBuffer& buffer, const VertexLayout& layout) const;

private:
    unsigned int m_ID;
};
