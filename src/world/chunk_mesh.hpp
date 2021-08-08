#pragma once

#include <glm/glm.hpp>

#include <vector>

#include "../gfx/vertex_array.hpp"
#include "../gfx/vertex_buffer.hpp"
#include "../common/types.hpp"

class Chunk;

struct ChunkVertex {
    glm::vec3 position;
    glm::vec2 tex_coords;
    glm::vec3 color;
};

struct ChunkIndex {
    u32 index;
};

struct ChunkFace {
    glm::vec3 position;
    size_t index_start;
    f32 distance;
};

class ChunkMesh {
public:
    void create();
    void destroy();

    void mesh(const Chunk& target, bool transparent);
    void sort();
    void render() const;

    [[nodiscard]]
    auto get_vertex_count() const -> u32;

    [[nodiscard]]
    auto get_index_count() const -> u32;

    friend struct BlockMeshParams;

private:
    std::vector<ChunkVertex> m_vertices;
    std::vector<ChunkIndex> m_indices;
    std::vector<ChunkFace> m_faces;

    u32 m_vertex_count { 0 };
    u32 m_index_count { 0 };

    VertexBuffer m_vbo, m_ibo;
    VertexArray m_vao;

    void reset();

    void sort_faces();
    void finalize_vertices();
    void finalize_indices();
};
