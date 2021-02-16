#pragma once

#include "../general/Mixins.hpp"

class Player : private NonCopyable {
public:
    float movementSpeed { 2.5f };
    float mouseSensitivity { 0.1f };

    bool Init();
    void Update(float dt);
};