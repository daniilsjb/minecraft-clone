#pragma once

#include <glad/glad.h>

#include <vector>
#include <cassert>

struct VertexElement {
    unsigned int type { 0 };
    unsigned int count { 0 };
    unsigned char normalized { 0 };
    unsigned int offset { 0 };

    VertexElement() = default;
    VertexElement(unsigned int t_type, unsigned int t_count, unsigned char t_normalized, unsigned int t_offset);

    static auto size_of(unsigned int type) -> unsigned int;
};

class VertexLayout {
public:
    template<typename T>
    void push_attribute(unsigned int count, unsigned char normalized = false) {
        assert(false);
    }

    auto get_elements() const -> const std::vector<VertexElement>& {
        return m_elements;
    }

    auto get_stride() const -> unsigned int {
        return m_stride;
    }

private:
    std::vector<VertexElement> m_elements;
    unsigned int m_stride { 0 };

    void push_typed_attribute(unsigned int type, unsigned int count, unsigned char normalized);
};

template<>
inline void VertexLayout::push_attribute<char>(unsigned int count, unsigned char normalized) {
    push_typed_attribute(GL_BYTE, count, normalized);
}

template<>
inline void VertexLayout::push_attribute<unsigned char>(unsigned int count, unsigned char normalized) {
    push_typed_attribute(GL_UNSIGNED_BYTE, count, normalized);
}

template<>
inline void VertexLayout::push_attribute<short>(unsigned int count, unsigned char normalized) {
    push_typed_attribute(GL_SHORT, count, normalized);
}

template<>
inline void VertexLayout::push_attribute<unsigned short>(unsigned int count, unsigned char normalized) {
    push_typed_attribute(GL_UNSIGNED_SHORT, count, normalized);
}

template<>
inline void VertexLayout::push_attribute<int>(unsigned int count, unsigned char normalized) {
    push_typed_attribute(GL_INT, count, normalized);
}

template<>
inline void VertexLayout::push_attribute<unsigned int>(unsigned int count, unsigned char normalized) {
    push_typed_attribute(GL_UNSIGNED_INT, count, normalized);
}

template<>
inline void VertexLayout::push_attribute<float>(unsigned int count, unsigned char normalized) {
    push_typed_attribute(GL_FLOAT, count, normalized);
}

template<>
inline void VertexLayout::push_attribute<double>(unsigned int count, unsigned char normalized) {
    push_typed_attribute(GL_DOUBLE, count, normalized);
}
