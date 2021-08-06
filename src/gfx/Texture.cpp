#include <stb_image.h>
#include <glad/glad.h>

#include <iostream>
#include <cassert>

#include "Texture.hpp"

Texture::Texture()
    : m_handle(0)
    , m_size(0, 0) {
}

Texture::Texture(unsigned char* pixels, size_t width, size_t height) {
    load_from_pixels(pixels, width, height);
}

Texture::Texture(const std::string& path) {
    load_from_path(path);
}

Texture ::~Texture() {
    destroy();
}

Texture::Texture(Texture&& other) noexcept
    : m_handle(other.m_handle)
    , m_size(other.m_size) {
    other.m_handle = 0;
    other.m_size = { 0, 0 };
}

Texture& Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {
        destroy();

        m_handle = other.m_handle;
        m_size = other.m_size;

        other.m_handle = 0;
        other.m_size = { 0, 0 };
    }

    return *this;
}

void Texture::create() {
    glGenTextures(1, &m_handle);
}

void Texture::destroy() {
    glDeleteTextures(1, &m_handle);
    m_handle = 0;
    m_size = { 0, 0 };
}

auto Texture::is_created() const -> bool {
    return m_handle != 0;
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, m_handle);
}

void Texture::load_from_pixels(unsigned char* pixels, size_t width, size_t height) {
    if (is_created()) {
        destroy();
    }
    create();

    m_size = { width, height };

    bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_size.x, m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
}

// TODO: Find a different way to signal texture errors
void Texture::load_from_path(const std::string& path) {
    stbi_set_flip_vertically_on_load(true);

    int num_channels = 0;
    unsigned char* data = stbi_load(path.c_str(), &m_size.x, &m_size.y, &num_channels, 0);
    if (data == nullptr) {
        assert(("Could not load texture at specified path", false));
    }

    load_from_pixels(data, static_cast<size_t>(m_size.x), static_cast<size_t>(m_size.y));
    stbi_image_free(data);
}

unsigned int Texture::get_handle() const {
    return m_handle;
}

auto Texture::get_width() const -> int {
    return m_size.x;
}

auto Texture::get_height() const -> int {
    return m_size.y;
}

auto Texture::get_size() const -> glm::ivec2 {
    return m_size;
}

auto Atlas::get_coordinates(const glm::ivec2& position) const -> AtlasCoords {
    const auto uv_min = glm::vec2 {
        (                     position.x) * m_sprite_size.x,
        (m_dimensions.y - 1 - position.y) * m_sprite_size.y
    };
    const auto uv_max = uv_min + static_cast<glm::vec2>(m_sprite_size);
    const auto size = static_cast<glm::vec2>(get_size());

    return {
        uv_min / size,
        uv_max / size
    };
}

auto Atlas::get_sprite_size() const -> glm::ivec2 {
    return m_sprite_size;
}

auto Atlas::get_dimensions() const -> glm::ivec2 {
    return m_dimensions;
}
