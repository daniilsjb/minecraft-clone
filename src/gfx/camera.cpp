#include "camera.hpp"

void Camera::init(u32 width, u32 height) {
    resize(width, height);
}

void Camera::update() {
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

void Camera::resize(u32 width, u32 height) {
    aspect_ratio = static_cast<f32>(width) / static_cast<f32>(height);
}
