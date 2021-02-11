#include "Shader.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <array>

Shader::Shader(const std::string& vertPath, const std::string& fragPath)
{
    m_ID = glCreateProgram();

    unsigned int vertID = CompileShader(GL_VERTEX_SHADER, ReadShaderFile(vertPath));
    unsigned int fragID = CompileShader(GL_FRAGMENT_SHADER, ReadShaderFile(fragPath));

    LinkProgram();
    glDeleteShader(vertID);
    glDeleteShader(fragID);
}

Shader::~Shader()
{
    glDeleteProgram(m_ID);
}

void Shader::Bind() const
{
    glUseProgram(m_ID);
}

void Shader::Unbind()
{
    glUseProgram(0);
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) const
{
    unsigned int shaderID = glCreateShader(type);

    const char* code = source.c_str();
    glShaderSource(shaderID, 1, &code, nullptr);
    glCompileShader(shaderID);

    int success = true;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        std::array<char, 512> log = { '\0' };
        glGetShaderInfoLog(shaderID, log.size(), nullptr, log.data());

        std::cerr << "[Shader] " << log.data() << '\n';
        std::cerr << "-- Type: Compilation\n";

        std::cerr << "-- Source: ";
        switch (type)
        {
            case GL_VERTEX_SHADER: std::cerr << "Vertex"; break;
            case GL_FRAGMENT_SHADER: std::cerr << "Fragment"; break;
            case GL_GEOMETRY_SHADER: std::cerr << "Geometry"; break;
            default: std::cerr << "Unknown"; break;
        }
        std::cerr << '\n';
    }

    glAttachShader(m_ID, shaderID);
    return shaderID;
}

void Shader::LinkProgram() const
{
    glLinkProgram(m_ID);

    int success = true;
    glGetProgramiv(m_ID, GL_LINK_STATUS, &success);

    if (!success)
    {
        std::array<char, 512> log = { '\0' };
        glGetProgramInfoLog(m_ID, log.size(), nullptr, log.data());

        std::cerr << "[Shader] " << log.data() << '\n';
        std::cerr << "-- Type: Linking\n";
    }
}

std::string Shader::ReadShaderFile(const std::string& path)
{
    std::string result;
    std::ifstream file(path, std::ios::in | std::ios::binary);

    if (!file)
    {
        std::cerr << "[Shader] Could not read file at '" << path << "'\n";
        std::cerr << "-- Type: Reading\n";
        return result;
    }

    file.seekg(0, std::ios::end);
    result.resize((size_t)file.tellg());
    file.seekg(0, std::ios::beg);

    file.read(&result[0], result.size());
    file.close();

    return result;
}
