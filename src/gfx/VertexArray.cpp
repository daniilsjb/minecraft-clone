#include "VertexArray.hpp"

VertexArray::VertexArray() {
    glGenVertexArrays(1, &m_handle);
}

VertexArray::VertexArray(VertexArray&& other) noexcept :
    m_handle(other.m_handle) {
    other.m_handle = 0;
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &m_handle);
}

void VertexArray::SetAttributes(const VertexBuffer& buffer, const VertexLayout& layout) const {
    Bind();
    buffer.Bind();

    const auto& elements = layout.GetElements();
    for (unsigned int i = 0; i < elements.size(); i++) {
        const auto& element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)element.offset);
    }
}

void VertexArray::Bind() const {
    glBindVertexArray(m_handle);
}

unsigned int VertexArray::GetHandle() const {
    return m_handle;
}
