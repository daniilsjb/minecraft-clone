#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

class Shader
{
public:
    Shader(const std::string& vertPath, const std::string& fragPath);
    ~Shader();

    void Bind() const;
    static void Unbind();

    template<typename T>
    void SetUniform(const std::string& name, const T& value)
    {
        static_assert(false);
    }

    template<>
    void SetUniform<float>(const std::string& name, const float& value)
    {
        unsigned int location = glGetUniformLocation(m_ID, name.c_str());
        glUniform1f(location, value);
    }

    template<>
    void SetUniform<int>(const std::string& name, const int& value)
    {
        unsigned int location = glGetUniformLocation(m_ID, name.c_str());
        glUniform1i(location, value);
    }

    template<>
    void SetUniform<unsigned int>(const std::string& name, const unsigned int& value)
    {
        unsigned int location = glGetUniformLocation(m_ID, name.c_str());
        glUniform1ui(location, value);
    }

    template<>
    void SetUniform<glm::vec2>(const std::string& name, const glm::vec2& value)
    {
        unsigned int location = glGetUniformLocation(m_ID, name.c_str());
        glUniform2fv(location, 1, glm::value_ptr(value));
    }

    template<>
    void SetUniform<glm::vec3>(const std::string& name, const glm::vec3& value)
    {
        unsigned int location = glGetUniformLocation(m_ID, name.c_str());
        glUniform3fv(location, 1, glm::value_ptr(value));
    }

    template<>
    void SetUniform<glm::vec4>(const std::string& name, const glm::vec4& value)
    {
        unsigned int location = glGetUniformLocation(m_ID, name.c_str());
        glUniform4fv(location, 1, glm::value_ptr(value));
    }

    template<>
    void SetUniform<glm::mat2>(const std::string& name, const glm::mat2& value)
    {
        unsigned int location = glGetUniformLocation(m_ID, name.c_str());
        glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }

    template<>
    void SetUniform<glm::mat3>(const std::string& name, const glm::mat3& value)
    {
        unsigned int location = glGetUniformLocation(m_ID, name.c_str());
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }

    template<>
    void SetUniform<glm::mat4>(const std::string& name, const glm::mat4& value)
    {
        unsigned int location = glGetUniformLocation(m_ID, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }

private:
    unsigned int m_ID;

    unsigned int CompileShader(unsigned int type, const std::string& source) const;
    void LinkProgram() const;

    static std::string ReadShaderFile(const std::string& path);
};