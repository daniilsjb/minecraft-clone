#include "VertexArray.hpp"

VertexArray::VertexArray(const VertexBuffer& buffer, const VertexLayout& layout) {
    Create();
    Attributes(buffer, layout);
}

VertexArray::~VertexArray() {
    Destroy();
}

VertexArray::VertexArray(VertexArray&& other) noexcept
    : m_handle(other.m_handle) {
    other.m_handle = 0;
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept {
    if (this != &other) {
        Destroy();

        m_handle = other.m_handle;
        other.m_handle = 0;
    }

    return *this;
}

void VertexArray::Create() {
    glGenVertexArrays(1, &m_handle);
}

void VertexArray::Destroy() {
    glDeleteVertexArrays(1, &m_handle);
    m_handle = 0;
}

auto VertexArray::IsCreated() const -> bool {
    return m_handle != 0;
}

void VertexArray::Bind() const {
    glBindVertexArray(m_handle);
}

void VertexArray::Attributes(const VertexBuffer& buffer, const VertexLayout& layout) const {
    Bind();
    buffer.Bind();

    const auto& elements = layout.GetElements();
    for (unsigned int i = 0; i < elements.size(); i++) {
        const auto& element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)element.offset);
    }
}

auto VertexArray::GetHandle() const -> unsigned int {
    return m_handle;
}
