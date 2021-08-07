#include "VertexArray.hpp"

VertexArray::VertexArray(const VertexBuffer& buffer, const VertexLayout& layout) {
    create();
    attributes(buffer, layout);
}

VertexArray::~VertexArray() {
    destroy();
}

VertexArray::VertexArray(VertexArray&& other) noexcept
    : m_handle(other.m_handle) {
        other.m_handle = 0;
    }

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept {
    if (this != &other) {
        destroy();

        m_handle = other.m_handle;
        other.m_handle = 0;
    }

    return *this;
}

void VertexArray::create() {
    glGenVertexArrays(1, &m_handle);
}

void VertexArray::destroy() {
    glDeleteVertexArrays(1, &m_handle);
    m_handle = 0;
}

void VertexArray::bind() const {
    glBindVertexArray(m_handle);
}

void VertexArray::attributes(const VertexBuffer& buffer, const VertexLayout& layout) const {
    bind();
    buffer.bind();

    const auto& elements = layout.get_elements();
    for (u32 i = 0; i < elements.size(); i++) {
        const auto& element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(
            i,
            static_cast<i32>(element.count),
            element.type,
            element.normalized,
            static_cast<i32>(layout.get_stride()),
            reinterpret_cast<const void*>(element.offset)
        );
    }
}

auto VertexArray::is_created() const -> bool {
    return m_handle != 0;
}

auto VertexArray::get_handle() const -> u32 {
    return m_handle;
}
