#include "Renderer.hpp"
#include "Window.hpp"

void Renderer::init() {
    camera.init(State::window->get_width(), State::window->get_height());

    atlas.load_from_path("res/textures/block-atlas.png");

    shaders[SHADER_QUAD].load_from_path("res/shaders/quad.vs", "res/shaders/quad.fs");
    shaders[SHADER_PLANE].load_from_path("res/shaders/plane.vs", "res/shaders/plane.fs");
    shaders[SHADER_CHUNK].load_from_path("res/shaders/chunk.vs", "res/shaders/chunk.fs");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_vbo.create(GL_ARRAY_BUFFER);
    m_ibo.create(GL_ELEMENT_ARRAY_BUFFER);
    m_vao.create();
}

void Renderer::update() {
    camera.update();

    if (State::window->keyboard.is_pressed(GLFW_KEY_T)) {
        flags.wireframe = !flags.wireframe;
    }
}

void Renderer::begin() const {
    glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, flags.wireframe ? GL_LINE : GL_FILL);
}

void Renderer::end() const {
}

[[maybe_unused]]
void Renderer::render_quad() const {
    f32 vertices[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f, 
         0.5f,  0.5f,
        -0.5f,  0.5f,
    };

    u32 indices[] = {
        0, 1, 2,
        2, 3, 0,
    };

    m_vbo.buffer(static_cast<const void*>(vertices), sizeof(vertices));
    m_ibo.buffer(static_cast<const void*>(indices), sizeof(indices));

    VertexLayout layout;
    layout.push_attribute<f32>(2); // screen coordinates
    m_vao.attributes(m_vbo, layout);

    shaders[SHADER_QUAD].bind();
    shaders[SHADER_QUAD].set_uniform("u_color", glm::vec3(0.560f, 0.701f, 0.737f));

    m_vao.bind();
    m_ibo.bind();

    glDisable(GL_CULL_FACE);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glEnable(GL_CULL_FACE);
}

[[maybe_unused]]
void Renderer::render_plane() const {
    auto [min, max] = atlas.get_coordinates({ 1, 0 });

    f32 vertices[] = {
        -0.5f, -0.5f,   min.x, min.y,
         0.5f, -0.5f,   max.x, min.y, 
         0.5f,  0.5f,   max.x, max.y,
        -0.5f,  0.5f,   min.x, max.y,
    };

    u32 indices[] = {
        0, 1, 2,
        2, 3, 0,
    };

    VertexLayout layout;
    layout.push_attribute<f32>(2); // screen coordinates
    layout.push_attribute<f32>(2); // texture coordinates

    m_vbo.buffer(static_cast<const void*>(vertices), sizeof(vertices));
    m_ibo.buffer(static_cast<const void*>(indices), sizeof(indices));
    m_vao.attributes(m_vbo, layout);

    shaders[SHADER_PLANE].bind();
    shaders[SHADER_PLANE].set_uniform("u_atlas", 0);
    shaders[SHADER_PLANE].set_uniform("u_proj", camera.projection);
    shaders[SHADER_PLANE].set_uniform("u_view", camera.view);
    shaders[SHADER_PLANE].set_uniform("u_model", glm::mat4(1.0f));

    atlas.bind();

    m_vao.bind();
    m_ibo.bind();

    glDisable(GL_CULL_FACE);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glEnable(GL_CULL_FACE);
}
