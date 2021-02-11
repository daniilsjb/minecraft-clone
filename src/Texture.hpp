#pragma once

#include <string>

class Texture
{
public:
    Texture(const std::string& path);
    ~Texture();

    void Bind() const;
    static void Unbind();

private:
    unsigned int m_ID;
};