#include <glad/glad.h>
#include <stb_image.h>

#include <iostream>

#include "Texture.hpp"

Texture::Texture(const std::string& path) :
    m_ID(0)
{
    glGenTextures(1, &m_ID);
    glBindTexture(GL_TEXTURE_2D, m_ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    int width = 0, height = 0, numChannels = 0;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &numChannels, 0);
    if (!data)
    {
        std::cerr << "[Texture] Could not load texture at '" << path << "'\n";
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_ID);
}

void Texture::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
