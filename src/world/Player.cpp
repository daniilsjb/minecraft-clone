#include "Player.hpp"
#include "../State.hpp"
#include "../gfx/Renderer.hpp"
#include "../gfx/Window.hpp"
#include "../world/World.hpp"
#include "../general/Ray.hpp"

constexpr auto ray_intersection = [](const glm::ivec3& position) {
    return State::world->GetBlock(position).id != BLOCK_AIR;
};

void Player::Init() {
}

void Player::Update(float dt) {
    if (State::window->keyboard.IsDown(GLFW_KEY_W)) {
        State::renderer->camera.position += State::renderer->camera.direction * dt * movement_speed;
        State::world->SetCenter(PositionToBlock(State::renderer->camera.position));
    }
    if (State::window->keyboard.IsDown(GLFW_KEY_S)) {
        State::renderer->camera.position -= State::renderer->camera.direction * dt * movement_speed;
        State::world->SetCenter(PositionToBlock(State::renderer->camera.position));
    }
    if (State::window->keyboard.IsDown(GLFW_KEY_A)) {
        State::renderer->camera.position -= State::renderer->camera.right * dt * movement_speed;
        State::world->SetCenter(PositionToBlock(State::renderer->camera.position));
    }
    if (State::window->keyboard.IsDown(GLFW_KEY_D)) {
        State::renderer->camera.position += State::renderer->camera.right * dt * movement_speed;
        State::world->SetCenter(PositionToBlock(State::renderer->camera.position));
    }
    if (State::window->keyboard.IsDown(GLFW_KEY_SPACE)) {
        State::renderer->camera.position += glm::vec3 { 0.0f, 1.0f, 0.0f } * dt * movement_speed;
    }

    glm::ivec3 block = PositionToBlock(State::renderer->camera.position);
    glm::ivec3 offset = BlockToOffset(block);

    changed_block = (this->block != block);
    changed_offset = (this->offset != offset);

    this->block = block;
    this->offset = offset;

    if (State::window->keyboard.IsPressed(GLFW_KEY_LEFT)) {
        selected_block.id--;
        if (selected_block.id == BLOCK_AIR) {
            selected_block.id = BLOCK_COUNT - 1;
        }
    }

    if (State::window->keyboard.IsPressed(GLFW_KEY_RIGHT)) {
        selected_block.id++;
        if (selected_block.id == BLOCK_COUNT) {
            selected_block.id = BLOCK_AIR + 1;
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
            State::world->SetBlock(rayhit->position + DirectionToVector(rayhit->face), selected_block);
        }
    }

    State::renderer->camera.yaw += (float)State::window->mouse.delta.x * mouse_sensitivity;
    State::renderer->camera.pitch -= (float)State::window->mouse.delta.y * mouse_sensitivity;
}

void Player::Destroy() {
}
