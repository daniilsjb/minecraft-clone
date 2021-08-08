#include "window.hpp"

#include <glad/glad.h>

#include <cassert>

#include "../world/world.hpp"
#include "renderer.hpp"

static void APIENTRY on_opengl_log_callback(GLenum source, GLenum type, u32 id, GLenum severity, GLsizei length, const char* message, const void* user_param) {
    std::cerr << "[OpenGL] " << message << '\n';

    std::cerr << "-- Source: ";
    switch (source) {
        case GL_DEBUG_SOURCE_API: std::cerr << "API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: std::cerr << "Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cerr << "Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY: std::cerr << "Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION: std::cerr << "Application"; break;
        case GL_DEBUG_SOURCE_OTHER: std::cerr << "Other"; break;
        default: std::cerr << "Unknown"; break;
    }
    std::cerr << '\n';

    std::cerr << "-- Type: ";
    switch (type) {
        case GL_DEBUG_TYPE_ERROR: std::cerr << "Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cerr << "Deprecated Behavior"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: std::cerr << "Undefined Behavior"; break;
        case GL_DEBUG_TYPE_PORTABILITY: std::cerr << "Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE: std::cerr << "Performance"; break;
        case GL_DEBUG_TYPE_MARKER: std::cerr << "Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP: std::cerr << "Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP: std::cerr << "Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER: std::cerr << "Other"; break;
        default: std::cerr << "Unknown"; break;
    }
    std::cerr << '\n';

    std::cerr << "-- Severity: ";
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH: std::cerr << "High"; break;
        case GL_DEBUG_SEVERITY_MEDIUM: std::cerr << "Medium"; break;
        case GL_DEBUG_SEVERITY_LOW: std::cerr << "Low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cerr << "Notification"; break;
        default: std::cerr << "Unknown"; break;
    }
    std::cerr << '\n';
}

static void on_glfw_error_callback(int error, const char* description) {
    std::cerr << "[GLFW] " << description << '\n';

    std::cerr << "-- Type: ";
    switch (error) {
        case GLFW_NOT_INITIALIZED: std::cerr << "Not Initialized"; break;
        case GLFW_NO_CURRENT_CONTEXT: std::cerr << "No Current Context"; break;
        case GLFW_INVALID_ENUM: std::cerr << "Invalid Enum"; break;
        case GLFW_INVALID_VALUE: std::cerr << "Invalid Value"; break;
        case GLFW_OUT_OF_MEMORY: std::cerr << "Out of Memory"; break;
        case GLFW_API_UNAVAILABLE: std::cerr << "API Unavailable"; break;
        case GLFW_VERSION_UNAVAILABLE: std::cerr << "Version Unavailable"; break;
        case GLFW_PLATFORM_ERROR: std::cerr << "Platform Error"; break;
        case GLFW_FORMAT_UNAVAILABLE: std::cerr << "Format Unavailable"; break;
        default: std::cerr << "Unknown"; break;
    }
    std::cerr << '\n';
}

static void on_resize_callback(GLFWwindow* handle, int width, int height) {
    reinterpret_cast<Window*>(glfwGetWindowUserPointer(handle))->on_resize(handle, u32(width), u32(height));
}

static void on_key_callback(GLFWwindow* handle, int key, int scancode, int action, int mods) {
    reinterpret_cast<Window*>(glfwGetWindowUserPointer(handle))->on_key(handle, key, scancode, action, mods);
}

static void on_cursor_callback(GLFWwindow* handle, double x, double y) {
    reinterpret_cast<Window*>(glfwGetWindowUserPointer(handle))->on_cursor(handle, x, y);
}

static void on_mouse_callback(GLFWwindow* handle, int button, int action, int mods) {
    reinterpret_cast<Window*>(glfwGetWindowUserPointer(handle))->on_mouse(handle, button, action, mods);
}

void Keyboard::update() {
    for (auto& key : keys) {
        key.pressed = key.down && !key.last;
        key.released = !key.down && key.last;
        key.last = key.down;
    }
}

bool Keyboard::is_down(u32 id) const {
    return keys[id].down;
}

bool Keyboard::is_pressed(u32 id) const {
    return keys[id].pressed;
}

bool Keyboard::is_released(u32 id) const {
    return keys[id].released;
}

void Mouse::update() {
    for (auto& button : buttons) {
        button.pressed = button.down && !button.last;
        button.released = !button.down && button.last;
        button.last = button.down;
    }
}

bool Mouse::is_down(u32 id) const {
    return buttons[id].down;
}

bool Mouse::is_pressed(u32 id) const {
    return buttons[id].pressed;
}

bool Mouse::is_released(u32 id) const {
    return buttons[id].released;
}

Window::Window(const std::string& name, u32 width, u32 height) {
    create(name, width, height);
}

Window::~Window() {
    destroy();
}

// TODO: Find a different way to signal window errors
void Window::create(const std::string& name, u32 width, u32 height) {
    m_width = width;
    m_height = height;

    glfwSetErrorCallback(on_glfw_error_callback);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    m_handle = glfwCreateWindow(i32(m_width), i32(m_height), name.c_str(), nullptr, nullptr);
    if (m_handle == nullptr) {
        assert(("Could not create a window", false));
    }

    glfwMakeContextCurrent(m_handle);
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        assert(("Could not load symbols via GLAD", false));
    }

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(on_opengl_log_callback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);

    glfwSetWindowUserPointer(m_handle, this);
    glfwSetWindowSizeCallback(m_handle, on_resize_callback);
    glfwSetKeyCallback(m_handle, on_key_callback);
    glfwSetCursorPosCallback(m_handle, on_cursor_callback);
    glfwSetMouseButtonCallback(m_handle, on_mouse_callback);

    glfwSetInputMode(m_handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwGetCursorPos(m_handle, &mouse.pos.x, &mouse.pos.y);

    glfwSwapInterval(1);
}

void Window::destroy() {
    glfwDestroyWindow(m_handle);
    glfwTerminate();
}

void Window::start() {
    auto t1 = static_cast<f32>(glfwGetTime());
    auto t2 = static_cast<f32>(glfwGetTime());
    auto dt = 0.0f;

    while (!glfwWindowShouldClose(m_handle)) {
        t2 = static_cast<f32>(glfwGetTime());
        dt = t2 - t1;
        t1 = t2;

        State::window->update();
        State::renderer->update();
        State::world->update(dt);

        State::world->prepare_render();

        State::renderer->begin();
        State::world->render();
        State::renderer->end();

        mouse.delta = { 0.0f, 0.0f };

        glfwSwapBuffers(m_handle);
        glfwPollEvents();
    }
}

void Window::update() {
    keyboard.update();
    mouse.update();

    if (keyboard.is_pressed(GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(m_handle, true);
    }
}

void Window::on_resize(GLFWwindow* handle, u32 width, u32 height) {
    m_width = width;
    m_height = height;

    glViewport(0, 0, i32(m_width), i32(m_height));
    State::renderer->camera.resize(width, height);
}

void Window::on_key(GLFWwindow* handle, i32 key, i32 scancode, i32 action, i32 mods) {
    if (key < 0) {
        return;
    }

    switch (action) {
        case GLFW_PRESS: keyboard.keys[static_cast<size_t>(key)].down = true; break;
        case GLFW_RELEASE: keyboard.keys[static_cast<size_t>(key)].down = false; break;
        default: break;
    }
}

void Window::on_cursor(GLFWwindow* handle, f64 x, f64 y) {
    glm::dvec2 new_pos = { x, y };

    // Accumulate the delta between callback invocations
    mouse.delta += new_pos - mouse.pos;
    mouse.pos = new_pos;
}

void Window::on_mouse(GLFWwindow* handle, i32 button, i32 action, i32 mods) {
    if (button < 0) {
        return;
    }

    switch (action) {
        case GLFW_PRESS: mouse.buttons[static_cast<size_t>(button)].down = true; break;
        case GLFW_RELEASE: mouse.buttons[static_cast<size_t>(button)].down = false; break;
        default: break;
    }
}

auto Window::get_handle() const -> GLFWwindow* {
    return m_handle;
}

auto Window::get_width() const -> u32 {
    return m_width;
}

auto Window::get_height() const -> u32 {
    return m_height;
}
