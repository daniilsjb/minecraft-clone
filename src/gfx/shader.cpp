#include "shader.hpp"

#include <glad/glad.h>

#include <cstdio>
#include <fstream>

Shader::Shader()
    : m_handle(0) {
}

Shader::Shader(std::string_view vert_path, std::string_view frag_path)
    : m_handle(0) {
    load_from_path(vert_path, frag_path);
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

Shader::~Shader() {
    destroy();
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

static void log_and_fail(std::string_view adverb, std::string_view reason) {
    std::fprintf(stderr, "[Shader] Could not %s shader: %s\n",
        adverb.data(),
        reason.data()
    );
    std::exit(EXIT_FAILURE);
}

using glGetStatus = void (APIENTRYP)(GLuint, GLenum, GLint*);
using glGetReason = void (APIENTRYP)(GLuint, GLsizei, GLsizei*, GLchar*);

struct BuildStepCheck {
    glGetStatus get_status;
    glGetReason get_reason;
    const char* adverb;
    u32 status_type;
    u32 target_id;
};

static void verify_step(const BuildStepCheck& step) {
    i32 success;
    step.get_status(step.target_id, step.status_type, &success);

    if (!success) {
        std::array<char, 512> log = { '\0' };
        step.get_reason(step.target_id, log.size(), nullptr, log.data());

        log_and_fail(step.adverb, log.data());
    }
}


static inline void verify_compilation(u32 shader_id) {
    verify_step(BuildStepCheck {
        .get_status = glGetShaderiv,
        .get_reason = glGetShaderInfoLog,
        .adverb = "compile",
        .status_type = GL_COMPILE_STATUS,
        .target_id = shader_id,
    });
}

static inline void verify_linking(u32 handle) {
    verify_step(BuildStepCheck {
        .get_status = glGetProgramiv,
        .get_reason = glGetProgramInfoLog,
        .adverb = "link",
        .status_type = GL_LINK_STATUS,
        .target_id = handle,
    });
}

static auto compile_shader(u32 type, std::string_view source) -> u32 {
    const auto shader_id = glCreateShader(type);

    const char* code = source.data();
    glShaderSource(shader_id, 1, &code, nullptr);
    glCompileShader(shader_id);

    verify_compilation(shader_id);

    return shader_id;
}

void Shader::load_from_string(std::string_view vert, std::string_view frag) {
    create();

    const auto vert_id = compile_shader(GL_VERTEX_SHADER, vert);
    const auto frag_id = compile_shader(GL_FRAGMENT_SHADER, frag);

    glAttachShader(m_handle, vert_id);
    glAttachShader(m_handle, frag_id);

    glLinkProgram(m_handle);
    verify_linking(m_handle);

    glDeleteShader(vert_id);
    glDeleteShader(frag_id);
}

static auto read_shader_file(std::string_view path) -> std::string {
    std::string result;
    std::ifstream file(path.data(), std::ios::in | std::ios::binary);

    if (!file) {
        log_and_fail("read", path.data());
    }

    file.seekg(0, std::ios::end);
    result.resize(size_t(file.tellg()));
    file.seekg(0, std::ios::beg);

    file.read(&result[0], i64(result.size()));
    file.close();

    return result;
}

void Shader::load_from_path(std::string_view vert_path, std::string_view frag_path) {
    const std::string vert_source = read_shader_file(vert_path);
    const std::string frag_source = read_shader_file(frag_path);
    load_from_string(vert_source, frag_source);
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

auto Shader::get_location(std::string_view name) const -> i32 {
    return glGetUniformLocation(m_handle, name.data());
}

void Shader::set_uniform(std::string_view name, const f32 value) const {
    glUniform1f(get_location(name), value);
}

void Shader::set_uniform(std::string_view name, const i32 value) const {
    glUniform1i(get_location(name), value);
}

void Shader::set_uniform(std::string_view name, const u32 value) const {
    glUniform1ui(get_location(name), value);
}

void Shader::set_uniform(std::string_view name, const glm::vec2 value) const {
    glUniform2fv(get_location(name), 1, glm::value_ptr(value));
}

void Shader::set_uniform(std::string_view name, const glm::vec3 value) const {
    glUniform3fv(get_location(name), 1, glm::value_ptr(value));
}

void Shader::set_uniform(std::string_view name, const glm::vec4 value) const {
    glUniform4fv(get_location(name), 1, glm::value_ptr(value));
}

void Shader::set_uniform(std::string_view name, const glm::mat2 value) const {
    glUniformMatrix2fv(get_location(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set_uniform(std::string_view name, const glm::mat3 value) const {
    glUniformMatrix3fv(get_location(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set_uniform(std::string_view name, const glm::mat4 value) const {
    glUniformMatrix4fv(get_location(name), 1, GL_FALSE, glm::value_ptr(value));
}
