#include "VertexLayout.hpp"

VertexElement::VertexElement() = default;

VertexElement::VertexElement(unsigned int type, unsigned int count, unsigned char normalized, unsigned int offset) :
    type(type), count(count), normalized(normalized), offset(offset) {
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