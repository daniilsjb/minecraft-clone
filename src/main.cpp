#include "state.hpp"
#include "gfx/window.hpp"
#include "gfx/renderer.hpp"
#include "world/world.hpp"

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
