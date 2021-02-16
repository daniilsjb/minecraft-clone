#pragma once

#include <glm/glm.hpp>

#include <vector>

#include "../gfx/VertexArray.hpp"
#include "../gfx/VertexBuffer.hpp"

class Chunk;

struct ChunkVertex {
    glm::vec3 position;
    glm::vec2 texCoords;
};

struct ChunkIndex {
    unsigned int index;
};

struct ChunkMesh {
    Chunk* chunk { nullptr };

    std::vector<ChunkVertex> vertices;
    std::vector<ChunkIndex> indices;

    unsigned int vertexCount { 0 };
    unsigned int indexCount { 0 };

    VertexBuffer vbo, ibo;
    VertexArray vao;

    ChunkMesh(Chunk* chunk);

    void Mesh();
    void Render() const;

private:
    void FinalizeVertices();
    void FinalizeIndices();
};
