#pragma once

#include <glm/glm.hpp>

#include <vector>

#include "../gfx/VertexArray.hpp"
#include "../gfx/VertexBuffer.hpp"

class Chunk;

struct ChunkVertex {
    glm::vec3 position;
    glm::vec2 tex_coords;
    glm::vec3 color;
};

struct ChunkIndex {
    unsigned int index;
};

struct ChunkFace {
    glm::vec3 position;
    size_t index_start;
    float distance;
};

class ChunkMesh {
public:
    void Create();
    void Destroy();

    void Mesh(const Chunk& target, bool transparent);
    void Sort();
    void Render() const;

    auto GetVertexCount() const -> unsigned int;
    auto GetIndexCount() const -> unsigned int;

    friend struct BlockMeshParams;

private:
    std::vector<ChunkVertex> m_vertices;
    std::vector<ChunkIndex> m_indices;
    std::vector<ChunkFace> m_faces;

    unsigned int m_vertex_count { 0 };
    unsigned int m_index_count { 0 };

    VertexBuffer m_vbo, m_ibo;
    VertexArray m_vao;

    void Reset();

    void SortFaces();
    void FinalizeVertices();
    void FinalizeIndices();
};
