#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Camera {
    glm::mat4 projection;
    glm::mat4 view;

    glm::vec3 position { 0.0f, 80.0f, 0.0f };
    glm::vec3 direction { 0.0f, 0.0f, 0.0f };
    glm::vec3 up { 0.0f, 0.0f, 0.0f };
    glm::vec3 right { 0.0f, 0.0f, 0.0f };

    float fov { 75.0f };
    float aspectRatio { 0.0f };

    float yaw { 0.0f };
    float pitch { 0.0f };

    float near { 0.01f };
    float far { 1000.0f };

    void Init(int width, int height);
    void Update(float dt);

    void Resize(int width, int height);
};
