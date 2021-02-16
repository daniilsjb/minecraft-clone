#include <iostream>

#include "State.hpp"
#include "gfx/Window.hpp"
#include "gfx/Renderer.hpp"
#include "world/World.hpp"

int main() {
    // The main window with OpenGL context
    Window window;
    State::window = &window;
    if (!window.Init("Minecraft Clone", 1280, 720)) {
        return EXIT_FAILURE;
    }

    // The renderer and all associated rendering data
    Renderer renderer;
    State::renderer = &renderer;
    if (!renderer.Init()) {
        return EXIT_FAILURE;
    }

    // The world of the game instance
    World world;
    State::world = &world;
    if (!world.Init()) {
        return EXIT_FAILURE;
    }

    // Start the game
    window.Start();
    return EXIT_SUCCESS;
}
