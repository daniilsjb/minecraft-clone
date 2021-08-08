#pragma once

#include <glm/glm.hpp>

#include <string>

#include "../common/types.hpp"

class Shader {
public:
    Shader();
    Shader(const std::string& vert_path, const std::string& frag_path);

    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    void destroy();

    void load_from_string(const std::string& vert, const std::string& frag);
    void load_from_path(const std::string& vert_path, const std::string& frag_path);

    void bind() const;

    [[nodiscard]]
    auto is_created() const -> bool;

    [[nodiscard]]
    auto get_handle() const -> u32;

    void set_uniform(const std::string& name, f32 value) const;
    void set_uniform(const std::string& name, i32 value) const;
    void set_uniform(const std::string& name, u32 value) const;
    void set_uniform(const std::string& name, const glm::vec2& value) const;
    void set_uniform(const std::string& name, const glm::vec3& value) const;
    void set_uniform(const std::string& name, const glm::vec4& value) const;
    void set_uniform(const std::string& name, const glm::mat2& value) const;
    void set_uniform(const std::string& name, const glm::mat3& value) const;
    void set_uniform(const std::string& name, const glm::mat4& value) const;

    [[nodiscard]]
    auto get_location(const std::string& name) const -> i32;

private:
    u32 m_handle;

    void create();

    [[nodiscard]]
    auto attach_shader(u32 type, const std::string& source) const -> u32;

    void link_program() const;
};