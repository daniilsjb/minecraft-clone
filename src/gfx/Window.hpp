#pragma once

#include <GLFW/glfw3.h>

#include <array>
#include <string>

#include "../State.hpp"
#include "../general/Mixins.hpp"

struct KeyState {
    bool down;     // The key is currently held down
    bool pressed;  // The key has just been pressed
    bool released; // The key has just been released
    bool last;     // The key was held down during the last frame
};

struct Keyboard {
    void Update(float dt);

    bool IsDown(unsigned int id) const;
    bool IsPressed(unsigned int id) const;
    bool IsReleased(unsigned int id) const;

    std::array<KeyState, GLFW_KEY_LAST + 1> keys {};
};

struct Mouse {
    void Update(float dt);

    bool IsDown(unsigned int id) const;
    bool IsPressed(unsigned int id) const;
    bool IsReleased(unsigned int id) const;

    std::array<KeyState, GLFW_MOUSE_BUTTON_LAST + 1> buttons {};

    double posX { 0.0 }, posY { 0.0 };
    double deltaX { 0.0 }, deltaY { 0.0 };
};

class Window : private NonCopyable {
public:
    ~Window();

    bool Init(const std::string& name, int width, int height);
    void Start();
    void Update(float dt);

    void OnResize(GLFWwindow* handle, int width, int height);
    void OnKey(GLFWwindow* handle, int key, int scancode, int action, int mods);
    void OnCursor(GLFWwindow* handle, double x, double y);
    void OnMouse(GLFWwindow* handle, int button, int action, int mods);

    GLFWwindow* GetHandle() const;
    int GetWidth() const;
    int GetHeight() const;

    Keyboard keyboard;
    Mouse mouse;

private:
    GLFWwindow* m_handle { nullptr };
    int m_width { 0 }, m_height { 0 };
};
