#pragma once

#include <glm/glm.hpp>

#include <array>

#include "../State.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"

enum Shaders {
    SHADER_NONE = -1,
    SHADER_QUAD,
    SHADER_PLANE,
    SHADER_CHUNK,
    SHADER_COUNT
};

class Renderer {
public:
    // The global camera
    Camera camera;

    // The block atlas
    Atlas atlas;

    // The array of all available and pre-loaded shaders
    std::array<Shader, SHADER_COUNT> shaders;

    // Background color used when clearing the screen
    glm::vec4 clear_color { 0.1f, 0.6f, 0.6f, 1.0f };

    struct {
        // If true, all graphics are drawn in wireframe mode
        bool wireframe : 1;
    } flags {};

    void Init();
    void Update(float dt);

    void Begin() const;
    void End() const;

    void RenderQuad() const;
    void RenderPlane() const;

private:
    VertexBuffer m_vbo, m_ibo;
    VertexArray m_vao;
};