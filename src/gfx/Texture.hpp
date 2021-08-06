#pragma once

#include <glm/glm.hpp>

#include <string>
#include <tuple>

class Texture {
public:
    Texture();
    Texture(unsigned char* pixels, size_t width, size_t height);
    explicit Texture(const std::string& path);

    ~Texture();

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    void create();
    void destroy();

    auto is_created() const -> bool;

    void bind() const;
    void load_from_pixels(unsigned char* pixels, size_t width, size_t height);
    void load_from_path(const std::string& path);

    auto get_handle() const -> unsigned int;
    auto get_width() const -> int;
    auto get_height() const -> int;
    auto get_size() const -> glm::ivec2;

private:
    unsigned int m_handle;
    glm::ivec2 m_size;
};

using AtlasCoords = std::tuple<glm::vec2, glm::vec2>;

class Atlas : public Texture {
public:
    auto get_coordinates(const glm::ivec2& position) const -> AtlasCoords;

    auto get_sprite_size() const -> glm::ivec2;
    auto get_dimensions() const -> glm::ivec2;

private:
    // The size of each individual sprite within the atlas
    glm::ivec2 m_sprite_size { 16, 16 };

    // The number of sprites within the atlas
    glm::ivec2 m_dimensions { 16, 16 };
};
