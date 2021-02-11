#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <array>

#include "Game.hpp"

Game::Game() :
    m_simpleShader("res/shaders/simple.vert", "res/shaders/simple.frag")
{
}

Game::~Game() = default;

void Game::Start()
{
    //std::array<float, 30> vertices = {
    //    // Position     // Colors
    //    0.5f, -0.5f,    1.0f, 0.2f, 0.1f,
    //    0.5f,  0.5f,    1.0f, 0.6f, 0.1f,
    //   -0.5f, -0.5f,    0.1f, 0.6f, 0.2f,

    //   -0.5f,  0.5f,    0.2f, 0.3f, 0.6f,
    //   -0.5f, -0.5f,    0.1f, 0.6f, 0.2f,
    //    0.5f,  0.5f,    1.0f, 0.6f, 0.1f,
    //};

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };

    VertexLayout layout;
    layout.PushAttribute<float>(3);

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

    m_simpleVAO.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
