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

    void Create();
    void Destroy();

    auto IsCreated() const -> bool;

    void Bind() const;
    void LoadFromPixels(unsigned char* pixels, size_t width, size_t height);
    void LoadFromPath(const std::string& path);

    auto GetHandle() const -> unsigned int;
    auto GetWidth() const -> int;
    auto GetHeight() const -> int;
    auto GetSize() const -> glm::ivec2;

private:
    unsigned int m_handle;
    glm::ivec2 m_size;
};

using AtlasCoords = std::tuple<glm::vec2, glm::vec2>;

class Atlas : public Texture {
public:
    auto GetCoordinates(const glm::ivec2& position) const -> AtlasCoords;

    auto GetSpriteSize() const -> glm::ivec2;
    auto GetDimensions() const -> glm::ivec2;

private:
    // The size of each individual sprite within the atlas
    glm::ivec2 m_sprite_size { 16, 16 };

    // The number of sprites within the atlas
    glm::ivec2 m_dimensions { 16, 16 };
};
