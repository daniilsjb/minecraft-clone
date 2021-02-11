#pragma once

#include "VertexArray.hpp"
#include "Shader.hpp"

class Game
{
public:
    Game();
    ~Game();

    Game(const Game& other) = delete;
    Game(const Game&& other) = delete;
    Game& operator=(const Game& other) = delete;
    Game& operator=(const Game&& other) = delete;

    void Start();
    void Update(double dt);
    void Render();

private:
    VertexArray m_simpleVAO;
    VertexBuffer m_simpleVBO;

    Shader m_simpleShader;
};