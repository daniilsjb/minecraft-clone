#pragma once

#include "Block.hpp"

class Player {
public:
    float movementSpeed { 5.0f };
    float mouseSensitivity { 0.2f };

    Block selectedBlock { BLOCK_DIRT };

    void Init();
    void Update(float dt);
    void Destroy();
};
