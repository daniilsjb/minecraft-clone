#include "Window.hpp"

#include <glad/glad.h>

#include <algorithm>
#include <iostream>
#include <cassert>

#include "../world/World.hpp"
#include "Renderer.hpp"

static void APIENTRY OnOpenGLLogCallback(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam) {
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
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cerr << "Notifaction"; break;
        default: std::cerr << "Unknown"; break;
    }
    std::cerr << '\n';
}

static void OnGlfwErrorCallback(int error, const char* description) {
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

static void OnResizeCallback(GLFWwindow* handle, int width, int height) {
    reinterpret_cast<Window*>(glfwGetWindowUserPointer(handle))->OnResize(handle, width, height);
}

static void OnKeyCallback(GLFWwindow* handle, int key, int scancode, int action, int mods) {
    reinterpret_cast<Window*>(glfwGetWindowUserPointer(handle))->OnKey(handle, key, scancode, action, mods);
}

static void OnCursorCallback(GLFWwindow* handle, double x, double y) {
    reinterpret_cast<Window*>(glfwGetWindowUserPointer(handle))->OnCursor(handle, x, y);
}

static void OnMouseCallback(GLFWwindow* handle, int button, int action, int mods) {
    reinterpret_cast<Window*>(glfwGetWindowUserPointer(handle))->OnMouse(handle, button, action, mods);
}

void Keyboard::Update(float dt) {
    for (auto& key : keys) {
        key.pressed = key.down && !key.last;
        key.released = !key.down && key.last;
        key.last = key.down;
    }
}

bool Keyboard::IsDown(unsigned int id) const {
    return keys[id].down;
}

bool Keyboard::IsPressed(unsigned int id) const {
    return keys[id].pressed;
}

bool Keyboard::IsReleased(unsigned int id) const {
    return keys[id].released;
}

void Mouse::Update(float dt) {
    for (auto& button : buttons) {
        button.pressed = button.down && !button.last;
        button.released = !button.down && button.last;
        button.last = button.down;
    }
}

bool Mouse::IsDown(unsigned int id) const {
    return buttons[id].down;
}

bool Mouse::IsPressed(unsigned int id) const {
    return buttons[id].pressed;
}

bool Mouse::IsReleased(unsigned int id) const {
    return buttons[id].released;
}

Window::Window(const std::string& name, int width, int height) {
    Create(name, width, height);
}

Window::~Window() {
    Destroy();
}

// TODO: Find a different way to signal window errors
void Window::Create(const std::string& name, int width, int height) {
    m_width = width;
    m_height = height;

    glfwSetErrorCallback(OnGlfwErrorCallback);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    m_handle = glfwCreateWindow(m_width, m_height, name.c_str(), nullptr, nullptr);
    if (m_handle == nullptr) {
        assert(("Could not create a window", false));
    }

    glfwMakeContextCurrent(m_handle);
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        assert(("Could not load symbols via GLAD", false));
    }

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(OnOpenGLLogCallback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);

    glfwSetWindowUserPointer(m_handle, this);
    glfwSetWindowSizeCallback(m_handle, OnResizeCallback);
    glfwSetKeyCallback(m_handle, OnKeyCallback);
    glfwSetCursorPosCallback(m_handle, OnCursorCallback);
    glfwSetMouseButtonCallback(m_handle, OnMouseCallback);

    glfwSetInputMode(m_handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwGetCursorPos(m_handle, &mouse.pos.x, &mouse.pos.y);

    glfwSwapInterval(1);
}

void Window::Destroy() {
    glfwDestroyWindow(m_handle);
    glfwTerminate();
}

void Window::Start() {
    auto t1 = static_cast<float>(glfwGetTime());
    auto t2 = static_cast<float>(glfwGetTime());
    auto dt = 0.0f;

    while (!glfwWindowShouldClose(m_handle)) {
        t2 = static_cast<float>(glfwGetTime());
        dt = t2 - t1;
        t1 = t2;

        State::window->Update(dt);
        State::renderer->Update(dt);
        State::world->Update(dt);

        State::world->PrepareRender();

        State::renderer->Begin();
        State::world->Render();
        State::renderer->End();

        mouse.delta = { 0.0f, 0.0f };

        glfwSwapBuffers(m_handle);
        glfwPollEvents();
    }
}

void Window::Update(float dt) {
    keyboard.Update(dt);
    mouse.Update(dt);

    if (keyboard.IsPressed(GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(m_handle, true);
    }
}

void Window::OnResize(GLFWwindow* handle, int width, int height) {
    m_width = width;
    m_height = height;

    glViewport(0, 0, m_width, m_height);
    State::renderer->camera.Resize(width, height);
}

void Window::OnKey(GLFWwindow* handle, int key, int scancode, int action, int mods) {
    if (key < 0) {
        return;
    }

    switch (action) {
        case GLFW_PRESS: keyboard.keys[key].down = true; break;
        case GLFW_RELEASE: keyboard.keys[key].down = false; break;
        default: break;
    }
}

void Window::OnCursor(GLFWwindow* handle, double x, double y) {
    glm::dvec2 new_pos = { x, y };

    // Accumulate the delta between callback invocations
    mouse.delta += new_pos - mouse.pos;
    mouse.pos = new_pos;
}

void Window::OnMouse(GLFWwindow* handle, int button, int action, int mods) {
    if (button < 0) {
        return;
    }

    switch (action) {
        case GLFW_PRESS: mouse.buttons[button].down = true; break;
        case GLFW_RELEASE: mouse.buttons[button].down = false; break;
        default: break;
    }
}

auto Window::GetHandle() const -> GLFWwindow* {
    return m_handle;
}

auto Window::GetWidth() const -> int {
    return m_width;
}

auto Window::GetHeight() const -> int {
    return m_height;
}
