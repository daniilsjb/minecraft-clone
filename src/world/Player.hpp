#pragma once

class Player {
public:
    float movementSpeed { 5.0f };
    float mouseSensitivity { 0.2f };

    void Init();
    void Update(float dt);
    void Destroy();
};
