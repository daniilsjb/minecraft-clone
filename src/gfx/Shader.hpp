#pragma once

#include <glm/glm.hpp>

#include <string>

class Shader {
public:
    Shader() = default;
    Shader(const std::string& vert_path, const std::string& frag_path);

    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    void Create();
    void Destroy();

    auto IsCreated() const -> bool;

    void Bind() const;
    void LoadFromString(const std::string& vert, const std::string& frag);
    void LoadFromPath(const std::string& vert_path, const std::string& frag_path);

    void SetUniform(const std::string& name, const float value) const;
    void SetUniform(const std::string& name, const int value) const;
    void SetUniform(const std::string& name, const unsigned int value) const;
    void SetUniform(const std::string& name, const glm::vec2& value) const;
    void SetUniform(const std::string& name, const glm::vec3& value) const;
    void SetUniform(const std::string& name, const glm::vec4& value) const;
    void SetUniform(const std::string& name, const glm::mat2& value) const;
    void SetUniform(const std::string& name, const glm::mat3& value) const;
    void SetUniform(const std::string& name, const glm::mat4& value) const;

    auto GetHandle() const -> unsigned int;
    auto GetLocation(const std::string& name) const -> int;

private:
    unsigned int m_handle { 0 };

    auto ReadShaderFile(const std::string& path) const -> std::string;

    auto AttachShader(unsigned int type, const std::string& source) const -> unsigned int;
    void LinkProgram() const;
};