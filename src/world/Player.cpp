#include "Player.hpp"
#include "../State.hpp"
#include "../gfx/Renderer.hpp"
#include "../gfx/Window.hpp"
#include "../world/World.hpp"
#include "../general/Ray.hpp"

#include <iostream>

constexpr auto ray_intersection = [](const glm::ivec3& position) {
    return State::world->GetBlock(position).id != BLOCK_AIR;
};

void Player::Init() {
}

void Player::Update(float dt) {
    if (State::window->keyboard.IsDown(GLFW_KEY_W)) {
        State::renderer->camera.position += State::renderer->camera.direction * dt * movementSpeed;
        State::world->SetCenter(PositionToBlock(State::renderer->camera.position));
    }
    if (State::window->keyboard.IsDown(GLFW_KEY_S)) {
        State::renderer->camera.position -= State::renderer->camera.direction * dt * movementSpeed;
        State::world->SetCenter(PositionToBlock(State::renderer->camera.position));
    }
    if (State::window->keyboard.IsDown(GLFW_KEY_A)) {
        State::renderer->camera.position -= State::renderer->camera.right * dt * movementSpeed;
        State::world->SetCenter(PositionToBlock(State::renderer->camera.position));
    }
    if (State::window->keyboard.IsDown(GLFW_KEY_D)) {
        State::renderer->camera.position += State::renderer->camera.right * dt * movementSpeed;
        State::world->SetCenter(PositionToBlock(State::renderer->camera.position));
    }
    if (State::window->keyboard.IsDown(GLFW_KEY_SPACE)) {
        State::renderer->camera.position += glm::vec3 { 0.0f, 1.0f, 0.0f } * dt * movementSpeed;
    }

    if (State::window->keyboard.IsPressed(GLFW_KEY_LEFT)) {
        selectedBlock.id++;
        if (selectedBlock.id == BLOCK_COUNT) {
            selectedBlock.id = BLOCK_AIR + 1;
        }
    }

    if (State::window->keyboard.IsPressed(GLFW_KEY_RIGHT)) {
        selectedBlock.id--;
        if (selectedBlock.id == BLOCK_AIR) {
            selectedBlock.id = BLOCK_COUNT - 1;
        }
    }

    float ray_reach = 6.0f;
    if (State::window->mouse.IsPressed(GLFW_MOUSE_BUTTON_1)) {
        auto rayhit = (Ray {
            State::renderer->camera.position,
            State::renderer->camera.direction
        }).Cast(ray_reach, ray_intersection);

        if (rayhit.has_value()) {
            State::world->SetBlock(rayhit->position, Block { BLOCK_AIR });
        }
    }
    if (State::window->mouse.IsPressed(GLFW_MOUSE_BUTTON_2)) {
        auto rayhit = (Ray {
            State::renderer->camera.position,
            State::renderer->camera.direction
        }).Cast(ray_reach, ray_intersection);

        if (rayhit.has_value()) {
            State::world->SetBlock(rayhit->position + DirectionToVector(rayhit->face), selectedBlock);
        }
    }

    State::renderer->camera.yaw += (float)State::window->mouse.delta.x * mouseSensitivity;
    State::renderer->camera.pitch -= (float)State::window->mouse.delta.y * mouseSensitivity;
}

void Player::Destroy() {
}
