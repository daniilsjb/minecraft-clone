#include "VertexLayout.hpp"

// TODO: Maybe there is a better way to avoid shadowing warning than to use a t_ prefix
VertexElement::VertexElement(unsigned int t_type, unsigned int t_count, unsigned char t_normalized, unsigned int t_offset) :
    type(t_type), count(t_count), normalized(t_normalized), offset(t_offset) {
}

unsigned int VertexElement::SizeOf(unsigned int type) {
    switch (type) {
        case GL_BYTE: return sizeof(char);
        case GL_UNSIGNED_BYTE: return sizeof(unsigned char);
        case GL_SHORT: return sizeof(short);
        case GL_UNSIGNED_SHORT: return sizeof(unsigned short);
        case GL_INT: return sizeof(int);
        case GL_UNSIGNED_INT: return sizeof(unsigned int);
        case GL_FLOAT: return sizeof(float);
        case GL_DOUBLE: return sizeof(double);
        default: return 0;
    }
}

void VertexLayout::PushTypedAttribute(unsigned int type, unsigned int count, unsigned char normalized) {
    m_elements.emplace_back(type, count, normalized, m_stride);
    m_stride += count * VertexElement::SizeOf(type);
}
