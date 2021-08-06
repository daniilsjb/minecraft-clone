#pragma once

#include "Block.hpp"

class Player {
public:
    float movement_speed { 5.0f };
    float mouse_sensitivity { 0.2f };

    Block selected_block { 1 };

    bool changed_block { false };
    glm::ivec3 block;

    bool changed_offset { false };
    glm::ivec3 offset;

    void init();
    void update(float dt);
    void destroy();
};
