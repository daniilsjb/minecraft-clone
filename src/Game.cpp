#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <stb_image.h>

#include <array>
#include <iostream>

#include "Game.hpp"

Game::Game() :
    m_simpleShader("res/shaders/simple.vert", "res/shaders/simple.frag"),
    m_simpleTexture("res/textures/packs/oak.png")
{
}

Game::~Game() = default;

void Game::Start()
{
    // clang-format off
    float vertices[] = {
         // Position (x,y,z)    Texture (u,v)
         // =================================
         // Back
        -0.5f, -0.5f, -0.5f,    0.0f,  0.5f,  // bottom-left
         0.5f, -0.5f, -0.5f,    0.25f, 0.5f,  // bottom-right
         0.5f,  0.5f, -0.5f,    0.25f, 0.0f,  // top-right
         0.5f,  0.5f, -0.5f,    0.25f, 0.0f,  // top-right
        -0.5f,  0.5f, -0.5f,    0.0f,  0.0f,  // top-left
        -0.5f, -0.5f, -0.5f,    0.0f,  0.5f,  // bottom-left

         // Front
        -0.5f, -0.5f,  0.5f,    0.0f,  1.0f, // bottom-left
         0.5f, -0.5f,  0.5f,    0.25f, 1.0f, // bottom-right
         0.5f,  0.5f,  0.5f,    0.25f, 0.5f, // top-right
         0.5f,  0.5f,  0.5f,    0.25f, 0.5f, // top-right
        -0.5f,  0.5f,  0.5f,    0.0f,  0.5f, // top-left
        -0.5f, -0.5f,  0.5f,    0.0f,  1.0f, // bottom-left

         // Top
        -0.5f,  0.5f,  0.5f,    0.25f, 0.5f, // bottom-left
         0.5f,  0.5f,  0.5f,    0.5f,  0.5f, // bottom-right
         0.5f,  0.5f, -0.5f,    0.5f,  0.0f, // top-right
         0.5f,  0.5f, -0.5f,    0.5f,  0.0f, // top-right
        -0.5f,  0.5f, -0.5f,    0.25f, 0.0f, // top-left
        -0.5f,  0.5f,  0.5f,    0.25f, 0.5f, // bottom-left

         // Bottom          
         0.5f, -0.5f,  0.5f,    0.25f, 1.0f, // bottom-left
        -0.5f, -0.5f,  0.5f,    0.5f,  1.0f, // bottom-right
        -0.5f, -0.5f, -0.5f,    0.5f,  0.5f, // top-right
        -0.5f, -0.5f, -0.5f,    0.5f,  0.5f, // top-right
         0.5f, -0.5f, -0.5f,    0.25f, 0.5f, // top-left
         0.5f, -0.5f,  0.5f,    0.25f, 1.0f, // bottom-left

         // East
         0.5f, -0.5f,  0.5f,    0.5f,  0.5f, // bottom-left
         0.5f, -0.5f, -0.5f,    0.75f, 0.5f, // bottom-right
         0.5f,  0.5f, -0.5f,    0.75f, 0.0f, // top-right
         0.5f,  0.5f, -0.5f,    0.75f, 0.0f, // top-right
         0.5f,  0.5f,  0.5f,    0.5f,  0.0f, // top-left
         0.5f, -0.5f,  0.5f,    0.5f,  0.5f, // bottom-left

         // West
        -0.5f, -0.5f, -0.5f,    0.5f,  1.0f, // bottom-left
        -0.5f, -0.5f,  0.5f,    0.75f, 1.0f, // bottom-right
        -0.5f,  0.5f,  0.5f,    0.75f, 0.5f, // top-right
        -0.5f,  0.5f,  0.5f,    0.75f, 0.5f, // top-right
        -0.5f,  0.5f, -0.5f,    0.5f,  0.5f, // top-left
        -0.5f, -0.5f, -0.5f,    0.5f,  1.0f, // bottom-left
    };
    // clang-format on

    VertexLayout layout;
    layout.PushAttribute<float>(3);
    layout.PushAttribute<float>(2);

    m_simpleVBO.Generate((const void*)vertices, sizeof(vertices));
    m_simpleVAO.AddBuffer(m_simpleVBO, layout);
}

void Game::Update(double dt)
{
}

void Game::Render()
{
    glClearColor(0.1f, 0.6f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)1280 / (float)720, 0.1f, 100.0f);
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(1.0f, 0.0f, 1.0f));

    m_simpleShader.Bind();
    m_simpleShader.SetUniform("u_projection", proj);
    m_simpleShader.SetUniform("u_view", view);
    m_simpleShader.SetUniform("u_model", model);
    m_simpleShader.SetUniform("u_texture", 0);

    m_simpleVAO.Bind();
    m_simpleTexture.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
