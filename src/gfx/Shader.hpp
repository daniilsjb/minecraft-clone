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

    void create();
    void destroy();

    auto is_created() const -> bool;

    void bind() const;
    void load_from_string(const std::string& vert, const std::string& frag);
    void load_from_path(const std::string& vert_path, const std::string& frag_path);

    void set_uniform(const std::string& name, const float value) const;
    void set_uniform(const std::string& name, const int value) const;
    void set_uniform(const std::string& name, const unsigned int value) const;
    void set_uniform(const std::string& name, const glm::vec2& value) const;
    void set_uniform(const std::string& name, const glm::vec3& value) const;
    void set_uniform(const std::string& name, const glm::vec4& value) const;
    void set_uniform(const std::string& name, const glm::mat2& value) const;
    void set_uniform(const std::string& name, const glm::mat3& value) const;
    void set_uniform(const std::string& name, const glm::mat4& value) const;

    auto get_handle() const -> unsigned int;
    auto get_location(const std::string& name) const -> int;

private:
    unsigned int m_handle { 0 };

    auto read_shader_file(const std::string& path) const -> std::string;

    auto attach_shader(unsigned int type, const std::string& source) const -> unsigned int;
    void link_program() const;
};