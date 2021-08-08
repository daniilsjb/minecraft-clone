#include "shader.hpp"

#include <glad/glad.h>

#include <cstdio>
#include <cassert>
#include <fstream>

Shader::Shader()
    : m_handle(0) {}

Shader::Shader(const std::string& vert_path, const std::string& frag_path)
    : m_handle(0) {
        load_from_path(vert_path, frag_path);
    }

Shader::~Shader() {
    destroy();
}

Shader::Shader(Shader&& other) noexcept
    : m_handle(other.m_handle) {
        other.m_handle = 0;
    }

Shader& Shader::operator=(Shader&& other) noexcept {
    if (this != &other) {
        destroy();

        m_handle = other.m_handle;
        other.m_handle = 0;
    }

    return *this;
}

void Shader::create() {
    if (!is_created()) {
        m_handle = glCreateProgram();
    }
}

void Shader::destroy() {
    glDeleteProgram(m_handle);
    m_handle = 0;
}

auto Shader::attach_shader(u32 type, const std::string& source) const -> u32 {
    const u32 shader_id = glCreateShader(type);

    const char* code = source.c_str();
    glShaderSource(shader_id, 1, &code, nullptr);
    glCompileShader(shader_id);

    int success;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

    if (!success) {
        std::array<char, 512> log = { '\0' };
        glGetShaderInfoLog(shader_id, log.size(), nullptr, log.data());

        std::cerr << "[Shader] " << log.data() << '\n';
        std::cerr << "-- Type: Compilation\n";

        std::cerr << "-- Source: ";
        switch (type) {
            case GL_VERTEX_SHADER: std::cerr << "Vertex"; break;
            case GL_FRAGMENT_SHADER: std::cerr << "Fragment"; break;
            default: std::cerr << "Unknown"; break;
        }
        std::cerr << '\n';

        // TODO: Find a better way to exit
        assert(("Could not compile shader", false));
    }

    glAttachShader(m_handle, shader_id);
    return shader_id;
}

void Shader::link_program() const {
    glLinkProgram(m_handle);

    int success = true;
    glGetProgramiv(m_handle, GL_LINK_STATUS, &success);

    if (!success) {
        std::array<char, 512> log = { '\0' };
        glGetProgramInfoLog(m_handle, log.size(), nullptr, log.data());

        std::cerr << "[Shader] " << log.data() << '\n';
        std::cerr << "-- Type: Linking\n";

        assert(("Could not link shader", false));
    }
}

void Shader::load_from_string(const std::string& vert, const std::string& frag) {
    create();

    const auto vert_id = attach_shader(GL_VERTEX_SHADER, vert);
    const auto frag_id = attach_shader(GL_FRAGMENT_SHADER, frag);

    link_program();
    glDeleteShader(vert_id);
    glDeleteShader(frag_id);
}

static auto read_shader_file(const std::string& path) -> std::string {
    std::string result;
    std::ifstream file(path, std::ios::in | std::ios::binary);

    if (!file) {
        fprintf(stderr, "[Shader] Could not read shader at '%s'\n", path.c_str());

        // TODO: Find better way to exit
        assert(("Could not read shader", false));
    }

    file.seekg(0, std::ios::end);
    result.resize(static_cast<size_t>(file.tellg()));
    file.seekg(0, std::ios::beg);

    file.read(&result[0], static_cast<i64>(result.size()));
    file.close();

    return result;
}

void Shader::load_from_path(const std::string& vert_path, const std::string& frag_path) {
    load_from_string(read_shader_file(vert_path), read_shader_file(frag_path));
}

void Shader::bind() const {
    glUseProgram(m_handle);
}

auto Shader::is_created() const -> bool {
    return m_handle != 0;
}

auto Shader::get_handle() const -> u32 {
    return m_handle;
}

void Shader::set_uniform(const std::string& name, const f32 value) const {
    glUniform1f(get_location(name), value);
}

void Shader::set_uniform(const std::string& name, const i32 value) const {
    glUniform1i(get_location(name), value);
}

void Shader::set_uniform(const std::string& name, const u32 value) const {
    glUniform1ui(get_location(name), value);
}

void Shader::set_uniform(const std::string& name, const glm::vec2& value) const {
    glUniform2fv(get_location(name), 1, glm::value_ptr(value));
}

void Shader::set_uniform(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(get_location(name), 1, glm::value_ptr(value));
}

void Shader::set_uniform(const std::string& name, const glm::vec4& value) const {
    glUniform4fv(get_location(name), 1, glm::value_ptr(value));
}

void Shader::set_uniform(const std::string& name, const glm::mat2& value) const {
    glUniformMatrix2fv(get_location(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set_uniform(const std::string& name, const glm::mat3& value) const {
    glUniformMatrix3fv(get_location(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set_uniform(const std::string& name, const glm::mat4& value) const {
    glUniformMatrix4fv(get_location(name), 1, GL_FALSE, glm::value_ptr(value));
}

auto Shader::get_location(const std::string& name) const -> int {
    return glGetUniformLocation(m_handle, name.c_str());
}
