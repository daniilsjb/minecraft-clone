#include <glad/glad.h>
#include <stb_image.h>

#include <iostream>

#include "Texture.hpp"

Texture::Texture()
    : m_handle(0)
    , m_size(0, 0) {
}

Texture::Texture(unsigned char* pixels, size_t width, size_t height) {
    LoadFromPixels(pixels, width, height);
}

Texture::Texture(const std::string& path) {
    LoadFromPath(path);
}

Texture ::~Texture() {
    Destroy();
}

Texture::Texture(Texture&& other) noexcept
    : m_handle(other.m_handle)
    , m_size(other.m_size) {
    other.m_handle = 0;
    other.m_size = { 0, 0 };
}

Texture& Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {
        Destroy();

        m_handle = other.m_handle;
        m_size = other.m_size;

        other.m_handle = 0;
        other.m_size = { 0, 0 };
    }

    return *this;
}

void Texture::Create() {
    glGenTextures(1, &m_handle);
}

void Texture::Destroy() {
    glDeleteTextures(1, &m_handle);
    m_handle = 0;
    m_size = { 0, 0 };
}

auto Texture::IsCreated() const -> bool {
    return m_handle != 0;
}

void Texture::Bind() const {
    glBindTexture(GL_TEXTURE_2D, m_handle);
}

void Texture::LoadFromPixels(unsigned char* pixels, size_t width, size_t height) {
    if (IsCreated()) {
        Destroy();
    }
    Create();

    m_size = { width, height };

    Bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_size.x, m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
}

void Texture::LoadFromPath(const std::string& path) {
    stbi_set_flip_vertically_on_load(true);

    int num_channels = 0;
    unsigned char* data = stbi_load(path.c_str(), &m_size.x, &m_size.y, &num_channels, 0);
    if (data == nullptr) {
        throw std::runtime_error("Could not load texture at '" + path + "'");
    }

    LoadFromPixels(data, m_size.x, m_size.y);
    stbi_image_free(data);
}

unsigned int Texture::GetHandle() const {
    return m_handle;
}

auto Texture::GetWidth() const -> int {
    return m_size.x;
}

auto Texture::GetHeight() const -> int {
    return m_size.y;
}

auto Texture::GetSize() const -> glm::ivec2 {
    return m_size;
}

auto Atlas::GetCoordinates(const glm::ivec2& position) const -> AtlasCoords {
    const auto uv_min = glm::vec2 {
        (                     position.x) * m_sprite_size.x,
        (m_dimensions.y - 1 - position.y) * m_sprite_size.y
    };
    const auto uv_max = uv_min + (glm::vec2)m_sprite_size;
    const auto size = (glm::vec2)GetSize();

    return {
        uv_min / size,
        uv_max / size
    };
}

auto Atlas::GetSpriteSize() const -> glm::ivec2 {
    return m_sprite_size;
}

auto Atlas::GetDimensions() const -> glm::ivec2 {
    return m_dimensions;
}
