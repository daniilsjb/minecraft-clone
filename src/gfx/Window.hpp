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

    void Update(float dt);

    bool IsDown(unsigned int id) const;
    bool IsPressed(unsigned int id) const;
    bool IsReleased(unsigned int id) const;
};

struct Mouse {
    std::array<KeyState, GLFW_MOUSE_BUTTON_LAST + 1> buttons {};

    glm::dvec2 pos { 0.0, 0.0 };
    glm::dvec2 delta { 0.0, 0.0 };

    void Update(float dt);

    bool IsDown(unsigned int id) const;
    bool IsPressed(unsigned int id) const;
    bool IsReleased(unsigned int id) const;
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

    void Create(const std::string& name, int width, int height);
    void Destroy();

    void Start();
    void Update(float dt);

    void OnResize(GLFWwindow* handle, int width, int height);
    void OnKey(GLFWwindow* handle, int key, int scancode, int action, int mods);
    void OnCursor(GLFWwindow* handle, double x, double y);
    void OnMouse(GLFWwindow* handle, int button, int action, int mods);

    auto GetHandle() const -> GLFWwindow*;
    auto GetWidth() const -> int;
    auto GetHeight() const -> int;

private:
    GLFWwindow* m_handle { nullptr };
    int m_width { 0 }, m_height { 0 };
};
