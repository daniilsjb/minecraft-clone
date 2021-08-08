#pragma once

#include <glad/glad.h>

#include <cassert>

struct VertexElement {
    u32 type { 0 };
    u32 count { 0 };
    u32 offset { 0 };
    bool normalized { false };

    VertexElement() = default;
    VertexElement(u32 t_type, u32 t_count, u32 t_offset, bool t_normalized);

    static auto size_of(u32 type) -> u32;
};

class VertexLayout {
public:
    template<typename T>
    void push_attribute(u32 count, u8 normalized = false) {
        assert(false);
    }

    [[nodiscard]]
    auto get_elements() const -> const std::vector<VertexElement>& {
        return m_elements;
    }

    [[nodiscard]]
    auto get_stride() const -> u32 {
        return m_stride;
    }

private:
    std::vector<VertexElement> m_elements;
    u32 m_stride { 0 };

    void push_typed_attribute(u32 type, u32 count, bool normalized);
};

template<>
inline void VertexLayout::push_attribute<i8>(u32 count, u8 normalized) {
    push_typed_attribute(GL_BYTE, count, normalized);
}

template<>
inline void VertexLayout::push_attribute<u8>(u32 count, u8 normalized) {
    push_typed_attribute(GL_UNSIGNED_BYTE, count, normalized);
}

template<>
inline void VertexLayout::push_attribute<i16>(u32 count, u8 normalized) {
    push_typed_attribute(GL_SHORT, count, normalized);
}

template<>
inline void VertexLayout::push_attribute<u16>(u32 count, u8 normalized) {
    push_typed_attribute(GL_UNSIGNED_SHORT, count, normalized);
}

template<>
inline void VertexLayout::push_attribute<i32>(u32 count, u8 normalized) {
    push_typed_attribute(GL_INT, count, normalized);
}

template<>
inline void VertexLayout::push_attribute<u32>(u32 count, u8 normalized) {
    push_typed_attribute(GL_UNSIGNED_INT, count, normalized);
}

template<>
inline void VertexLayout::push_attribute<f32>(u32 count, u8 normalized) {
    push_typed_attribute(GL_FLOAT, count, normalized);
}

template<>
inline void VertexLayout::push_attribute<f64>(u32 count, u8 normalized) {
    push_typed_attribute(GL_DOUBLE, count, normalized);
}
