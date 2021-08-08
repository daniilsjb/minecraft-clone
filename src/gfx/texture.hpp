#pragma once

#include <glm/glm.hpp>

#include <string>
#include <tuple>

#include "../common/types.hpp"

class Texture {
public:
    Texture();
    Texture(u8* pixels, u32 width, u32 height);
    explicit Texture(const std::string& path);

    ~Texture();

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    void destroy();

    void load_from_pixels(u8* pixels, u32 width, u32 height);
    void load_from_path(const std::string& path);

    void bind() const;

    [[nodiscard]]
    auto is_created() const -> bool;

    [[nodiscard]]
    auto get_handle() const -> u32;

    [[nodiscard]]
    auto get_size() const -> glm::uvec2;

private:
    u32 m_handle;
    glm::ivec2 m_size;

    void create();
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
