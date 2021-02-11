#pragma once

class VertexBuffer
{
public:
    VertexBuffer();
    VertexBuffer(const void* buffer, unsigned int size, unsigned int usage = GL_STATIC_DRAW);
    ~VertexBuffer();

    void Generate(const void* buffer, unsigned int size, unsigned int usage = GL_STATIC_DRAW);

    void Bind() const;
    static void Unbind();

private:
    unsigned int m_ID;
};