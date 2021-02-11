#include "VertexLayout.hpp"

VertexElement::VertexElement(unsigned int type, unsigned int count, unsigned char normalized) :
    type(type), count(count), normalized(normalized)
{
}

unsigned int VertexElement::GetSizeOf(unsigned int type)
{
    switch (type)
    {
        case GL_FLOAT: return sizeof(float);
        default: return 0;
    }
}

VertexLayout::VertexLayout() :
    m_stride(0)
{
}
