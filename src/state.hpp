#pragma once

class Window;
class Renderer;
class World;

struct State {
    static inline Window* window { nullptr };
    static inline Renderer* renderer { nullptr };
    static inline World* world { nullptr };
};
