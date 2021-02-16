#include "Player.hpp"
#include "../State.hpp"
#include "../gfx/Window.hpp"
#include "../gfx/Renderer.hpp"
#include "../world/World.hpp"

bool Player::Init() {
    return true;
}

void Player::Update(float dt) {
    if (State::window->keyboard.IsDown(GLFW_KEY_W)) {
        State::renderer->camera.position += State::renderer->camera.direction * dt * movementSpeed;
        State::world->SetCenter(glm::floor(State::renderer->camera.position));
    }
    if (State::window->keyboard.IsDown(GLFW_KEY_S)) {
        State::renderer->camera.position -= State::renderer->camera.direction * dt * movementSpeed;
        State::world->SetCenter(glm::floor(State::renderer->camera.position));
    }
    if (State::window->keyboard.IsDown(GLFW_KEY_A)) {
        State::renderer->camera.position -= State::renderer->camera.right * dt * movementSpeed;
        State::world->SetCenter(glm::floor(State::renderer->camera.position));
    }
    if (State::window->keyboard.IsDown(GLFW_KEY_D)) {
        State::renderer->camera.position += State::renderer->camera.right * dt * movementSpeed;
        State::world->SetCenter(glm::floor(State::renderer->camera.position));
    }

    State::renderer->camera.yaw += (float)State::window->mouse.deltaX * mouseSensitivity;
    State::renderer->camera.pitch -= (float)State::window->mouse.deltaY * mouseSensitivity;
}
