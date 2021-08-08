#pragma once

struct Camera {
    glm::mat4 projection;
    glm::mat4 view;

    glm::vec3 position { 0.0f, 80.0f, 0.0f };
    glm::vec3 direction { 0.0f, 0.0f, 0.0f };
    glm::vec3 up { 0.0f, 0.0f, 0.0f };
    glm::vec3 right { 0.0f, 0.0f, 0.0f };

    f32 fov { 75.0f };
    f32 aspect_ratio { 0.0f };

    f32 yaw { 0.0f };
    f32 pitch { 0.0f };

    f32 near { 0.01f };
    f32 far { 1000.0f };

    void init(u32 width, u32 height);
    void update();

    void resize(u32 width, u32 height);
};
