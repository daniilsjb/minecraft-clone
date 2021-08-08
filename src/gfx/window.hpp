#pragma once

#include <GLFW/glfw3.h>

#include "../state.hpp"

struct KeyState {
    bool down;     // The key is currently held down
    bool pressed;  // The key has just been pressed
    bool released; // The key has just been released
    bool last;     // The key was held down during the last frame
};

struct Keyboard {
    std::array<KeyState, GLFW_KEY_LAST + 1> keys {};

    void update();

    [[nodiscard]]
    auto is_down(u32 id) const -> bool;

    [[nodiscard]]
    auto is_pressed(u32 id) const -> bool;

    [[nodiscard]]
    auto is_released(u32 id) const -> bool;
};

struct Mouse {
    std::array<KeyState, GLFW_MOUSE_BUTTON_LAST + 1> buttons {};

    glm::dvec2 pos { 0.0, 0.0 };
    glm::dvec2 delta { 0.0, 0.0 };

    void update();

    [[nodiscard]]
    auto is_down(u32 id) const -> bool;

    [[nodiscard]]
    auto is_pressed(u32 id) const -> bool;

    [[nodiscard]]
    auto is_released(u32 id) const -> bool;
};

class Window {
public:
    Keyboard keyboard;
    Mouse mouse;

    Window() = default;
    Window(const std::string& name, u32 width, u32 height);

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    Window(Window&& other) = delete;
    Window& operator=(Window&& other) = delete;

    ~Window();

    void create(const std::string& name, u32 width, u32 height);
    void destroy();

    void start();
    void update();

    void on_resize(GLFWwindow* handle, u32 width, u32 height);
    void on_key(GLFWwindow* handle, i32 key, i32 scancode, i32 action, i32 mods);
    void on_cursor(GLFWwindow* handle, f64 x, f64 y);
    void on_mouse(GLFWwindow* handle, i32 button, i32 action, i32 mods);

    [[nodiscard]]
    auto get_handle() const -> GLFWwindow*;

    [[nodiscard]]
    auto get_width() const -> u32;

    [[nodiscard]]
    auto get_height() const -> u32;

private:
    GLFWwindow* m_handle { nullptr };
    u32 m_width { 0 }, m_height { 0 };
};
