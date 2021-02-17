#pragma once

#include <glm/glm.hpp>

#include <vector>

#include "../gfx/VertexArray.hpp"
#include "../gfx/VertexBuffer.hpp"

class Chunk;

struct ChunkVertex {
    glm::vec3 position;
    glm::vec2 tex_coords;
};

struct ChunkIndex {
    unsigned int index;
};

class ChunkMesh {
public:
    void Create();
    void Destroy();

    void Mesh(const Chunk& target);
    void Render() const;

    auto GetVertexCount() const -> unsigned int;
    auto GetIndexCount() const -> unsigned int;

    friend struct BlockMeshParams;

private:
    std::vector<ChunkVertex> m_vertices;
    std::vector<ChunkIndex> m_indices;

    unsigned int m_vertex_count { 0 };
    unsigned int m_index_count { 0 };

    VertexBuffer m_vbo, m_ibo;
    VertexArray m_vao;

    void Reset();

    void FinalizeVertices();
    void FinalizeIndices();
};
