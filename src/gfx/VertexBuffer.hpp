#pragma once

#include <glad/glad.h>

#include "../general/Mixins.hpp"

class VertexBuffer : private NonCopyable {
public:
    VertexBuffer(unsigned int type = GL_ARRAY_BUFFER);
    VertexBuffer(VertexBuffer&& other) noexcept;
    ~VertexBuffer();

    void Generate(const void* buffer, unsigned int size, unsigned int usage = GL_STATIC_DRAW) const;
    void Bind() const;

    unsigned int GetHandle() const;
    unsigned int GetType() const;

private:
    unsigned int m_handle { 0 };
    unsigned int m_type { 0 };
};