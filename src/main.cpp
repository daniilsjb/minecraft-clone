#include <iostream>

#include "State.hpp"
#include "gfx/Window.hpp"
#include "gfx/Renderer.hpp"
#include "world/World.hpp"

int main() {
    // The main window with OpenGL context
    Window window;
    State::window = &window;
    window.Create("Minecraft Clone", 1280, 720);

    // The renderer and all associated rendering data
    Renderer renderer;
    State::renderer = &renderer;
    renderer.Init();

    // The world of the game instance
    World world;
    State::world = &world;
    world.Create();

    // Start the game
    window.Start();
    return EXIT_SUCCESS;
}
