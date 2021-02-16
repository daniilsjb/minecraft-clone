#include "Shader.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <array>
#include <fstream>
#include <iostream>

Shader::Shader(const std::string& vertPath, const std::string& fragPath) {
    m_handle = glCreateProgram();

    unsigned int vertID = CompileShader(GL_VERTEX_SHADER, ReadShaderFile(vertPath));
    unsigned int fragID = CompileShader(GL_FRAGMENT_SHADER, ReadShaderFile(fragPath));

    LinkProgram();
    glDeleteShader(vertID);
    glDeleteShader(fragID);
}

Shader::Shader(Shader&& other) noexcept
    : m_handle(other.m_handle) {
    other.m_handle = 0;
}

Shader::~Shader() {
    glDeleteProgram(m_handle);
}

void Shader::Bind() const {
    glUseProgram(m_handle);
}

void Shader::SetUniform(const std::string& name, const float value) const {
    unsigned int location = glGetUniformLocation(m_handle, name.c_str());
    glUniform1f(location, value);
}

void Shader::SetUniform(const std::string& name, const int value) const {
    unsigned int location = glGetUniformLocation(m_handle, name.c_str());
    glUniform1i(location, value);
}

void Shader::SetUniform(const std::string& name, const unsigned int value) const {
    unsigned int location = glGetUniformLocation(m_handle, name.c_str());
    glUniform1ui(location, value);
}

void Shader::SetUniform(const std::string& name, const glm::vec2& value) const {
    unsigned int location = glGetUniformLocation(m_handle, name.c_str());
    glUniform2fv(location, 1, glm::value_ptr(value));
}

void Shader::SetUniform(const std::string& name, const glm::vec3& value) const {
    unsigned int location = glGetUniformLocation(m_handle, name.c_str());
    glUniform3fv(location, 1, glm::value_ptr(value));
}

void Shader::SetUniform(const std::string& name, const glm::vec4& value) const {
    unsigned int location = glGetUniformLocation(m_handle, name.c_str());
    glUniform4fv(location, 1, glm::value_ptr(value));
}

void Shader::SetUniform(const std::string& name, const glm::mat2& value) const {
    unsigned int location = glGetUniformLocation(m_handle, name.c_str());
    glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetUniform(const std::string& name, const glm::mat3& value) const {
    unsigned int location = glGetUniformLocation(m_handle, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetUniform(const std::string& name, const glm::mat4& value) const {
    unsigned int location = glGetUniformLocation(m_handle, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

unsigned int Shader::GetHandle() const {
    return m_handle;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) const {
    unsigned int shaderID = glCreateShader(type);

    const char* code = source.c_str();
    glShaderSource(shaderID, 1, &code, nullptr);
    glCompileShader(shaderID);

    int success = true;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

    if (!success) {
        std::array<char, 512> log = { '\0' };
        glGetShaderInfoLog(shaderID, log.size(), nullptr, log.data());

        std::cerr << "[Shader] " << log.data() << '\n';
        std::cerr << "-- Type: Compilation\n";

        std::cerr << "-- Source: ";
        switch (type) {
            case GL_VERTEX_SHADER: std::cerr << "Vertex"; break;
            case GL_FRAGMENT_SHADER: std::cerr << "Fragment"; break;
            default: std::cerr << "Unknown"; break;
        }
        std::cerr << '\n';
    }

    glAttachShader(m_handle, shaderID);
    return shaderID;
}

void Shader::LinkProgram() const {
    glLinkProgram(m_handle);

    int success = true;
    glGetProgramiv(m_handle, GL_LINK_STATUS, &success);

    if (!success) {
        std::array<char, 512> log = { '\0' };
        glGetProgramInfoLog(m_handle, log.size(), nullptr, log.data());

        std::cerr << "[Shader] " << log.data() << '\n';
        std::cerr << "-- Type: Linking\n";
    }
}

std::string Shader::ReadShaderFile(const std::string& path) {
    std::string result;
    std::ifstream file(path, std::ios::in | std::ios::binary);

    if (!file) {
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
