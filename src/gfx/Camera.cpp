#include "Camera.hpp"

#include <algorithm>

void Camera::init(int width, int height) {
    resize(width, height);
}

void Camera::update(float dt) {
    yaw = (yaw < 0.0f ? 360.0f : 0.0f) + glm::mod(yaw, 360.0f);
    pitch = std::clamp(pitch, -89.0f, 89.0f);

    direction = {
        cos(glm::radians(pitch)) * cos(glm::radians(yaw)),
        sin(glm::radians(pitch)),
        cos(glm::radians(pitch)) * sin(glm::radians(yaw))
    };
    direction = glm::normalize(direction);

    right = glm::normalize(glm::cross(direction, glm::vec3(0.0f, 1.0f, 0.0f)));
    up    = glm::normalize(glm::cross(right, direction));

    projection = glm::perspective(glm::radians(fov), aspect_ratio, near, far);
    view       = glm::lookAt(position, position + direction, up);
}

void Camera::resize(int width, int height) {
    aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
}
