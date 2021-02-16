#pragma once

#include <glm/glm.hpp>

#include <string>

#include "../general/Mixins.hpp"

class Shader : private NonCopyable {
public:
    Shader(const std::string& vertPath, const std::string& fragPath);
    Shader(Shader&& other) noexcept;
    ~Shader();

    void Bind() const;

    void SetUniform(const std::string& name, const float value) const;
    void SetUniform(const std::string& name, const int value) const;
    void SetUniform(const std::string& name, const unsigned int value) const;
    void SetUniform(const std::string& name, const glm::vec2& value) const;
    void SetUniform(const std::string& name, const glm::vec3& value) const;
    void SetUniform(const std::string& name, const glm::vec4& value) const;
    void SetUniform(const std::string& name, const glm::mat2& value) const;
    void SetUniform(const std::string& name, const glm::mat3& value) const;
    void SetUniform(const std::string& name, const glm::mat4& value) const;

    unsigned int GetHandle() const;

private:
    unsigned int CompileShader(unsigned int type, const std::string& source) const;
    void LinkProgram() const;

    static std::string ReadShaderFile(const std::string& path);

    unsigned int m_handle { 0 };
};