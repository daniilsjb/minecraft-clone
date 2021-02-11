#include <glad/glad.h>

#include <vector>

struct VertexElement
{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    VertexElement(unsigned int type, unsigned int count, unsigned char normalized);

    static unsigned int GetSizeOf(unsigned int type);
};

class VertexLayout
{
public:
    VertexLayout();

    template<typename T>
    void PushAttribute(unsigned int count)
    {
        static_assert(false);
    }

    template<>
    void PushAttribute<float>(unsigned int count)
    {
        m_elements.emplace_back(GL_FLOAT, count, false);
        m_stride += count * VertexElement::GetSizeOf(GL_FLOAT);
    }

    const std::vector<VertexElement>& GetElements() const
    {
        return m_elements;
    }

    unsigned int GetStride() const
    {
        return m_stride;
    }

private:
    std::vector<VertexElement> m_elements;
    unsigned int m_stride;
};
