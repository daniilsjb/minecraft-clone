#pragma once

#include <glm/glm.hpp>

#include <array>

#include "../State.hpp"
#include "../general/Mixins.hpp"
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

class Renderer : private NonCopyable {
public:
    Camera camera;

    Atlas atlas { "res/textures/block-atlas.png" };
    std::array<Shader, SHADER_COUNT> shaders {
        Shader("res/shaders/quad.vs", "res/shaders/quad.fs"),
        Shader("res/shaders/plane.vs", "res/shaders/plane.fs"),
        Shader("res/shaders/chunk.vs", "res/shaders/chunk.fs")
    };

    VertexBuffer vbo, ibo;
    VertexArray vao;

    glm::vec4 clearColor { 0.1f, 0.6f, 0.6f, 1.0f };

    struct {
        bool wireframe : 1;
    } flags { 0 };

    Renderer();
    ~Renderer();

    bool Init();
    void Update(float dt);

    void Begin() const;
    void End() const;

    void RenderQuad(const glm::vec3& color) const;
    void RenderPlane(const glm::ivec2& position, const glm::mat4& model) const;
};