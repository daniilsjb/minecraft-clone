#pragma once

class Shader {
public:
    explicit Shader();
    explicit Shader(std::string_view vert_path, std::string_view frag_path);

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    Shader(Shader&&) noexcept;
    Shader& operator=(Shader&&) noexcept;

    ~Shader();

    void destroy();

    void load_from_string(std::string_view vert, std::string_view frag);
    void load_from_path(std::string_view vert_path, std::string_view frag_path);

    void bind() const;

    [[nodiscard]]
    auto is_created() const -> bool;

    [[nodiscard]]
    auto get_handle() const -> u32;

    void set_uniform(std::string_view name, f32 value) const;
    void set_uniform(std::string_view name, i32 value) const;
    void set_uniform(std::string_view name, u32 value) const;
    void set_uniform(std::string_view name, glm::vec2 value) const;
    void set_uniform(std::string_view name, glm::vec3 value) const;
    void set_uniform(std::string_view name, glm::vec4 value) const;
    void set_uniform(std::string_view name, glm::mat2 value) const;
    void set_uniform(std::string_view name, glm::mat3 value) const;
    void set_uniform(std::string_view name, glm::mat4 value) const;

    [[nodiscard]]
    auto get_location(std::string_view name) const -> i32;

private:
    u32 m_handle;

    void create();
};