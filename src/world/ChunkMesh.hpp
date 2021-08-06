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
    void create();
    void destroy();

    void mesh(const Chunk& target, bool transparent);
    void sort();
    void render() const;

    auto get_vertex_count() const -> unsigned int;
    auto get_index_count() const -> unsigned int;

    friend struct BlockMeshParams;

private:
    std::vector<ChunkVertex> m_vertices;
    std::vector<ChunkIndex> m_indices;
    std::vector<ChunkFace> m_faces;

    unsigned int m_vertex_count { 0 };
    unsigned int m_index_count { 0 };

    VertexBuffer m_vbo, m_ibo;
    VertexArray m_vao;

    void reset();

    void sort_faces();
    void finalize_vertices();
    void finalize_indices();
};
