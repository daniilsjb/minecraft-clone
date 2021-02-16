#include <glad/glad.h>
#include <stb_image.h>

#include <iostream>

#include "Texture.hpp"

Texture::Texture(const std::string& path) {
    glGenTextures(1, &m_handle);
    glBindTexture(GL_TEXTURE_2D, m_handle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    stbi_set_flip_vertically_on_load(true);

    int numChannels = 0;
    unsigned char* data = stbi_load(path.c_str(), &m_size.x, &m_size.y, &numChannels, 0);
    if (data == nullptr) {
        std::cerr << "[Texture] Could not load texture at '" << path << "'\n";
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_size.x, m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
}

Texture::Texture(Texture&& other) noexcept
    : m_handle(other.m_handle), m_size(other.m_size) {
    other.m_handle = 0;
}

Texture ::~Texture() {
    glDeleteTextures(1, &m_handle);
}

void Texture::Bind() const {
    glBindTexture(GL_TEXTURE_2D, m_handle);
}

unsigned int Texture::GetHandle() const {
    return m_handle;
}

int Texture::GetWidth() const {
    return m_size.x;
}

int Texture::GetHeight() const {
    return m_size.y;
}

std::tuple<glm::vec2, glm::vec2> Atlas::GetCoordinates(const glm::ivec2& position) const {
    glm::vec2 min = glm::vec2 {
        position.x * spriteSize.x,
        (spriteNum.y - 1 - position.y) * spriteSize.y
    };

    return {
        min / (glm::vec2)(m_size),
        (min + (glm::vec2)(spriteSize)) / (glm::vec2)(m_size)
    };
}
