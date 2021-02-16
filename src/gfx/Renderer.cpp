#include "Renderer.hpp"
#include "Window.hpp"

#include <glad/glad.h>

Renderer::Renderer() :
    vbo(GL_ARRAY_BUFFER),
    ibo(GL_ELEMENT_ARRAY_BUFFER) {
}

Renderer::~Renderer() = default;

bool Renderer::Init() {
    if (!camera.Init(State::window->GetWidth(), State::window->GetHeight())) {
        return false;
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return true;
}

void Renderer::Update(float dt) {
    camera.Update(dt);

    if (State::window->keyboard.IsPressed(GLFW_KEY_T)) {
        flags.wireframe = !flags.wireframe;
    }
}

void Renderer::Begin() const {
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, flags.wireframe ? GL_LINE : GL_FILL);
}

void Renderer::End() const {
}

void Renderer::RenderQuad(const glm::vec3& color) const {
    float vertices[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f, 
         0.5f,  0.5f,
        -0.5f,  0.5f,
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0,
    };

    vbo.Generate((const void*)vertices, sizeof(vertices));
    ibo.Generate((const void*)indices, sizeof(indices));

    VertexLayout layout;
    layout.PushAttribute<float>(2);
    vao.SetAttributes(vbo, layout);

    shaders[SHADER_QUAD].Bind();
    shaders[SHADER_QUAD].SetUniform("u_color", color);

    vao.Bind();
    ibo.Bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void Renderer::RenderPlane(const glm::ivec2& position, const glm::mat4& model) const {
    auto [min, max] = atlas.GetCoordinates(position);

    float vertices[] = {
        -0.5f, -0.5f,   min.x, min.y,
         0.5f, -0.5f,   max.x, min.y, 
         0.5f,  0.5f,   max.x, max.y,
        -0.5f,  0.5f,   min.x, max.y,
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0,
    };

    VertexLayout layout;
    layout.PushAttribute<float>(2);
    layout.PushAttribute<float>(2);

    vbo.Generate((const void*)vertices, sizeof(vertices));
    ibo.Generate((const void*)indices, sizeof(indices));
    vao.SetAttributes(vbo, layout);

    shaders[SHADER_PLANE].Bind();
    shaders[SHADER_PLANE].SetUniform("u_atlas", 0);
    shaders[SHADER_PLANE].SetUniform("u_proj", camera.projection);
    shaders[SHADER_PLANE].SetUniform("u_view", camera.view);
    shaders[SHADER_PLANE].SetUniform("u_model", model);

    atlas.Bind();

    vao.Bind();
    ibo.Bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
