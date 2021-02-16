#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Camera {
    glm::mat4 projection;
    glm::mat4 view;

    glm::vec3 position { 0.0f, 0.0f, 0.0f };
    glm::vec3 direction { 0.0f, 0.0f, 0.0f };
    glm::vec3 up { 0.0f, 0.0f, 0.0f };
    glm::vec3 right { 0.0f, 0.0f, 0.0f };

    float fov { glm::half_pi<float>() };
    float aspectRatio { 0.0f };

    float yaw { 0.0f };
    float pitch { 0.0f };

    float near { 0.1f };
    float far { 100.0f };

    bool Init(int width, int height);
    void Update(float dt);

    void Resize(int width, int height);
};
