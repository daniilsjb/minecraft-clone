#pragma once

#include <glad/glad.h>

#include <vector>

struct VertexElement {
    unsigned int type { 0 };
    unsigned int count { 0 };
    unsigned char normalized { 0 };
    unsigned int offset { 0 };

    VertexElement() = default;
    VertexElement(unsigned int type, unsigned int count, unsigned char normalized, unsigned int offset);

    static unsigned int SizeOf(unsigned int type);
};

class VertexLayout {
public:
    template<typename T>
    void PushAttribute(unsigned int count, unsigned char normalized = false) {
        static_assert(false);
    }

    template<>
    void PushAttribute<char>(unsigned int count, unsigned char normalized) {
        PushTypedAttribute(GL_BYTE, count, normalized);
    }

    template<>
    void PushAttribute<unsigned char>(unsigned int count, unsigned char normalized) {
        PushTypedAttribute(GL_UNSIGNED_BYTE, count, normalized);
    }

    template<>
    void PushAttribute<short>(unsigned int count, unsigned char normalized) {
        PushTypedAttribute(GL_SHORT, count, normalized);
    }

    template<>
    void PushAttribute<unsigned short>(unsigned int count, unsigned char normalized) {
        PushTypedAttribute(GL_UNSIGNED_SHORT, count, normalized);
    }

    template<>
    void PushAttribute<int>(unsigned int count, unsigned char normalized) {
        PushTypedAttribute(GL_INT, count, normalized);
    }

    template<>
    void PushAttribute<unsigned int>(unsigned int count, unsigned char normalized) {
        PushTypedAttribute(GL_UNSIGNED_INT, count, normalized);
    }

    template<>
    void PushAttribute<float>(unsigned int count, unsigned char normalized) {
        PushTypedAttribute(GL_FLOAT, count, normalized);
    }

    template<>
    void PushAttribute<double>(unsigned int count, unsigned char normalized) {
        PushTypedAttribute(GL_DOUBLE, count, normalized);
    }

    const std::vector<VertexElement>& GetElements() const {
        return m_elements;
    }

    unsigned int GetStride() const {
        return m_stride;
    }

private:
    std::vector<VertexElement> m_elements;
    unsigned int m_stride { 0 };

    void PushTypedAttribute(unsigned int type, unsigned int count, unsigned char normalized);
};
