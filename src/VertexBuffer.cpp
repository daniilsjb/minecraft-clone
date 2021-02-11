#include <glad/glad.h>

#include "VertexBuffer.hpp"

VertexBuffer::VertexBuffer() :
    m_ID(0)
{
}

VertexBuffer::VertexBuffer(const void* buffer, unsigned int size, unsigned int usage) :
    m_ID(0)
{
    Generate(buffer, size, usage);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_ID);
}

void VertexBuffer::Generate(const void* buffer, unsigned int size, unsigned int usage)
{
    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    glBufferData(GL_ARRAY_BUFFER, size, buffer, usage);
}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}

void VertexBuffer::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}