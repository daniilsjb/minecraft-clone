#include "vertex_layout.hpp"

// TODO: Maybe there is a better way to avoid shadowing warning than to use a t_ prefix
VertexElement::VertexElement(u32 t_type, u32 t_count, u32 t_offset, bool t_normalized)
    : type(t_type)
    , count(t_count)
    , offset(t_offset)
    , normalized(t_normalized) {}

u32 VertexElement::size_of(u32 type) {
    switch (type) {
        case GL_BYTE: return sizeof(i8);
        case GL_UNSIGNED_BYTE: return sizeof(u8);
        case GL_SHORT: return sizeof(i16);
        case GL_UNSIGNED_SHORT: return sizeof(u16);
        case GL_INT: return sizeof(i32);
        case GL_UNSIGNED_INT: return sizeof(u32);
        case GL_FLOAT: return sizeof(f32);
        case GL_DOUBLE: return sizeof(f64);
        default: return 0;
    }
}

void VertexLayout::push_typed_attribute(u32 type, u32 count, bool normalized) {
    m_elements.emplace_back(type, count, m_stride, normalized);
    m_stride += count * VertexElement::size_of(type);
}
