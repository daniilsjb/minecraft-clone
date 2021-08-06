#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <array>
#include <string>

#include "../State.hpp"

struct KeyState {
    bool down;     // The key is currently held down
    bool pressed;  // The key has just been pressed
    bool released; // The key has just been released
    bool last;     // The key was held down during the last frame
};

struct Keyboard {
    std::array<KeyState, GLFW_KEY_LAST + 1> keys {};

    void update(float dt);

    auto is_down(unsigned int id) const -> bool;
    auto is_pressed(unsigned int id) const -> bool;
    auto is_released(unsigned int id) const -> bool;
};

struct Mouse {
    std::array<KeyState, GLFW_MOUSE_BUTTON_LAST + 1> buttons {};

    glm::dvec2 pos { 0.0, 0.0 };
    glm::dvec2 delta { 0.0, 0.0 };

    void update(float dt);

    auto is_down(unsigned int id) const -> bool;
    auto is_pressed(unsigned int id) const -> bool;
    auto is_released(unsigned int id) const -> bool;
};

class Window {
public:
    Keyboard keyboard;
    Mouse mouse;

    Window() = default;
    Window(const std::string& name, int width, int height);

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    Window(Window&& other) = delete;
    Window& operator=(Window&& other) = delete;

    ~Window();

    void create(const std::string& name, int width, int height);
    void destroy();

    void start();
    void update(float dt);

    void on_resize(GLFWwindow* handle, int width, int height);
    void on_key(GLFWwindow* handle, int key, int scancode, int action, int mods);
    void on_cursor(GLFWwindow* handle, double x, double y);
    void on_mouse(GLFWwindow* handle, int button, int action, int mods);

    auto get_handle() const -> GLFWwindow*;
    auto get_width() const -> int;
    auto get_height() const -> int;

private:
    GLFWwindow* m_handle { nullptr };
    int m_width { 0 }, m_height { 0 };
};
