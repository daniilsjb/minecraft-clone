#pragma once

#include <glm/glm.hpp>

#include <string>
#include <tuple>

class Texture {
public:
    Texture(const std::string& path);
    Texture(Texture&& other) noexcept;
    ~Texture();

    void Bind() const;

    unsigned int GetHandle() const;
    int GetWidth() const;
    int GetHeight() const;

protected:
    unsigned int m_handle { 0 };
    glm::ivec2 m_size { 0, 0 };
};

struct Atlas : public Texture {
    glm::ivec2 spriteSize { 16, 16 };
    glm::ivec2 spriteNum { 16, 16 };

    std::tuple<glm::vec2, glm::vec2> GetCoordinates(const glm::ivec2& position) const;
};
