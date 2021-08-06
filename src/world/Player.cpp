#include "Player.hpp"
#include "../State.hpp"
#include "../gfx/Renderer.hpp"
#include "../gfx/Window.hpp"
#include "../world/World.hpp"
#include "../common//Ray.hpp"

constexpr auto RAY_INTERSECTION = [](const glm::ivec3& position) {
    return State::world->get_block(position).id != BLOCK_AIR;
};

void Player::init() {
}

void Player::update(float dt) {
    if (State::window->keyboard.is_down(GLFW_KEY_W)) {
        State::renderer->camera.position += State::renderer->camera.direction * dt * movement_speed;
        State::world->set_center(position_to_block(State::renderer->camera.position));
    }
    if (State::window->keyboard.is_down(GLFW_KEY_S)) {
        State::renderer->camera.position -= State::renderer->camera.direction * dt * movement_speed;
        State::world->set_center(position_to_block(State::renderer->camera.position));
    }
    if (State::window->keyboard.is_down(GLFW_KEY_A)) {
        State::renderer->camera.position -= State::renderer->camera.right * dt * movement_speed;
        State::world->set_center(position_to_block(State::renderer->camera.position));
    }
    if (State::window->keyboard.is_down(GLFW_KEY_D)) {
        State::renderer->camera.position += State::renderer->camera.right * dt * movement_speed;
        State::world->set_center(position_to_block(State::renderer->camera.position));
    }
    if (State::window->keyboard.is_down(GLFW_KEY_SPACE)) {
        State::renderer->camera.position += glm::vec3 { 0.0f, 1.0f, 0.0f } * dt * movement_speed;
    }

    glm::ivec3 new_block = position_to_block(State::renderer->camera.position);
    glm::ivec3 new_offset = block_to_offset(new_block);

    changed_block = (this->block != new_block);
    changed_offset = (this->offset != new_offset);

    this->block = new_block;
    this->offset = new_offset;

    if (State::window->keyboard.is_pressed(GLFW_KEY_LEFT)) {
        selected_block.id--;
        if (selected_block.id == BLOCK_AIR) {
            selected_block.id = BLOCK_COUNT - 1;
        }
    }

    if (State::window->keyboard.is_pressed(GLFW_KEY_RIGHT)) {
        selected_block.id++;
        if (selected_block.id == BLOCK_COUNT) {
            selected_block.id = BLOCK_AIR + 1;
        }
    }

    float ray_reach = 6.0f;
    if (State::window->mouse.is_pressed(GLFW_MOUSE_BUTTON_1)) {
        auto ray_hit = (Ray {
            State::renderer->camera.position,
            State::renderer->camera.direction
        }).cast(ray_reach, RAY_INTERSECTION);

        if (ray_hit.has_value()) {
            State::world->set_block(ray_hit->position, Block { BLOCK_AIR });
        }
    }
    if (State::window->mouse.is_pressed(GLFW_MOUSE_BUTTON_2)) {
        auto ray_hit = (Ray {
            State::renderer->camera.position,
            State::renderer->camera.direction
        }).cast(ray_reach, RAY_INTERSECTION);

        if (ray_hit.has_value()) {
            // Cannot attach blocks to sprites
            if (!Blocks::data[State::world->get_block(ray_hit->position).id].sprite) {
                State::world->set_block(ray_hit->position + direction_to_vector(ray_hit->face), selected_block);
            }
        }
    }

    State::renderer->camera.yaw += static_cast<float>(State::window->mouse.delta.x) * mouse_sensitivity;
    State::renderer->camera.pitch -= static_cast<float>(State::window->mouse.delta.y) * mouse_sensitivity;
}

void Player::destroy() {
}
