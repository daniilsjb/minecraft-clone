#include "VertexArray.hpp"

VertexArray::VertexArray() :
    m_ID(0)
{
    glGenVertexArrays(1, &m_ID);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_ID);
}

void VertexArray::Bind() const
{
    glBindVertexArray(m_ID);
}

void VertexArray::Unbind()
{
    glBindVertexArray(0);
}

void VertexArray::AddBuffer(const VertexBuffer& buffer, const VertexLayout& layout) const
{
    Bind();
    buffer.Bind();

    unsigned int offset = 0;
    const auto& elements = layout.GetElements();

    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto& element = elements[i];

        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset);

        offset += element.count * VertexElement::GetSizeOf(element.type);
    }
}
