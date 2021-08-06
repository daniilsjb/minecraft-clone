#include "State.hpp"
#include "gfx/Window.hpp"
#include "gfx/Renderer.hpp"
#include "world/World.hpp"

auto main() -> int {
    // The main window with OpenGL context
    Window window;
    State::window = &window;
    window.create("Minecraft Clone", 1280, 720);

    // The renderer and all associated rendering data
    Renderer renderer;
    State::renderer = &renderer;
    renderer.init();

    // The world of the game instance
    World world;
    State::world = &world;
    world.create();

    // Start the game
    window.start();
    return EXIT_SUCCESS;
}
