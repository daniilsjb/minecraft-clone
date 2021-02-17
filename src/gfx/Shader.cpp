#include "Shader.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <array>
#include <fstream>
#include <iostream>

Shader::Shader(const std::string& vert_path, const std::string& frag_path) {
    m_handle = glCreateProgram();
    LoadFromPath(vert_path, frag_path);
}

Shader::~Shader() {
    Destroy();
}

Shader::Shader(Shader&& other) noexcept
    : m_handle(other.m_handle) {
    other.m_handle = 0;
}

Shader& Shader::operator=(Shader&& other) noexcept {
    if (this != &other) {
        Destroy();

        m_handle = other.m_handle;
        other.m_handle = 0;
    }

    return *this;
}

void Shader::Create() {
    m_handle = glCreateProgram();
}

void Shader::Destroy() {
    glDeleteProgram(m_handle);
    m_handle = 0;
}

auto Shader::IsCreated() const -> bool {
    return m_handle != 0;
}

void Shader::Bind() const {
    glUseProgram(m_handle);
}

void Shader::LoadFromString(const std::string& vert, const std::string& frag) {
    if (IsCreated()) {
        Destroy();
    }
    Create();

    const unsigned int vertID = AttachShader(GL_VERTEX_SHADER, vert);
    const unsigned int fragID = AttachShader(GL_FRAGMENT_SHADER, frag);

    LinkProgram();
    glDeleteShader(vertID);
    glDeleteShader(fragID);
}

void Shader::LoadFromPath(const std::string& vert_path, const std::string& frag_path) {
    LoadFromString(ReadShaderFile(vert_path), ReadShaderFile(frag_path));
}

void Shader::SetUniform(const std::string& name, const float value) const {
    glUniform1f(GetLocation(name), value);
}

void Shader::SetUniform(const std::string& name, const int value) const {
    glUniform1i(GetLocation(name), value);
}

void Shader::SetUniform(const std::string& name, const unsigned int value) const {
    glUniform1ui(GetLocation(name), value);
}

void Shader::SetUniform(const std::string& name, const glm::vec2& value) const {
    glUniform2fv(GetLocation(name), 1, glm::value_ptr(value));
}

void Shader::SetUniform(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(GetLocation(name), 1, glm::value_ptr(value));
}

void Shader::SetUniform(const std::string& name, const glm::vec4& value) const {
    glUniform4fv(GetLocation(name), 1, glm::value_ptr(value));
}

void Shader::SetUniform(const std::string& name, const glm::mat2& value) const {
    glUniformMatrix2fv(GetLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetUniform(const std::string& name, const glm::mat3& value) const {
    glUniformMatrix3fv(GetLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetUniform(const std::string& name, const glm::mat4& value) const {
    glUniformMatrix4fv(GetLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

unsigned int Shader::GetHandle() const {
    return m_handle;
}

auto Shader::GetLocation(const std::string& name) const -> unsigned int {
    return glGetUniformLocation(m_handle, name.c_str());
}

auto Shader::AttachShader(unsigned int type, const std::string& source) const -> unsigned int {
    const unsigned int shaderID = glCreateShader(type);

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

        throw std::runtime_error("Could not compile shader");
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

        throw std::runtime_error("Could not link shader");
    }
}

auto Shader::ReadShaderFile(const std::string& path) const -> std::string {
    std::string result;
    std::ifstream file(path, std::ios::in | std::ios::binary);

    if (!file) {
        std::cerr << "[Shader] Could not read file at '" << path << "'\n";
        std::cerr << "-- Type: Reading\n";

        throw std::runtime_error("Could not read shader");
    }

    file.seekg(0, std::ios::end);
    result.resize((size_t)file.tellg());
    file.seekg(0, std::ios::beg);

    file.read(&result[0], result.size());
    file.close();

    return result;
}
