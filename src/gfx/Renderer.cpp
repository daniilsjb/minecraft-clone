#include "Renderer.hpp"
#include "Window.hpp"

#include <glad/glad.h>

void Renderer::Init() {
    camera.Init(State::window->GetWidth(), State::window->GetHeight());

    atlas.LoadFromPath("res/textures/block-atlas.png");

    shaders[SHADER_QUAD].LoadFromPath("res/shaders/quad.vs", "res/shaders/quad.fs");
    shaders[SHADER_PLANE].LoadFromPath("res/shaders/plane.vs", "res/shaders/plane.fs");
    shaders[SHADER_CHUNK].LoadFromPath("res/shaders/chunk.vs", "res/shaders/chunk.fs");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_vbo.Create(GL_ARRAY_BUFFER);
    m_ibo.Create(GL_ELEMENT_ARRAY_BUFFER);
    m_vao.Create();
}

void Renderer::Update(float dt) {
    camera.Update(dt);

    if (State::window->keyboard.IsPressed(GLFW_KEY_T)) {
        flags.wireframe = !flags.wireframe;
    }
}

void Renderer::Begin() const {
    glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, flags.wireframe ? GL_LINE : GL_FILL);
}

void Renderer::End() const {
}

void Renderer::RenderQuad() const {
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

    m_vbo.Buffer((const void*)vertices, sizeof(vertices));
    m_ibo.Buffer((const void*)indices, sizeof(indices));

    VertexLayout layout;
    layout.PushAttribute<float>(2);
    m_vao.Attributes(m_vbo, layout);

    shaders[SHADER_QUAD].Bind();
    shaders[SHADER_QUAD].SetUniform("u_color", glm::vec3(0.560f, 0.701f, 0.737f));

    m_vao.Bind();
    m_ibo.Bind();

    glDisable(GL_CULL_FACE);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glEnable(GL_CULL_FACE);
}

void Renderer::RenderPlane() const {
    auto [min, max] = atlas.GetCoordinates({ 1, 0 });

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

    m_vbo.Buffer((const void*)vertices, sizeof(vertices));
    m_ibo.Buffer((const void*)indices, sizeof(indices));
    m_vao.Attributes(m_vbo, layout);

    shaders[SHADER_PLANE].Bind();
    shaders[SHADER_PLANE].SetUniform("u_atlas", 0);
    shaders[SHADER_PLANE].SetUniform("u_proj", camera.projection);
    shaders[SHADER_PLANE].SetUniform("u_view", camera.view);
    shaders[SHADER_PLANE].SetUniform("u_model", glm::mat4(1.0f));

    atlas.Bind();

    m_vao.Bind();
    m_ibo.Bind();

    glDisable(GL_CULL_FACE);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glEnable(GL_CULL_FACE);
}
