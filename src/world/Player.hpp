#pragma once

#include "Block.hpp"
#include "../common/Types.hpp"

class Player {
public:
    f32 movement_speed { 5.0f };
    f32 mouse_sensitivity { 0.2f };

    Block selected_block { 1 };

    bool changed_block { false };
    glm::ivec3 block;

    bool changed_offset { false };
    glm::ivec3 offset;

    void init();
    void update(f32 dt);
    void destroy();
};
