#include "BlockMesh.hpp"
#include "ChunkMesh.hpp"
#include "Block.hpp"

#include <array>

constexpr std::array<float, 8 * 3> CUBE_COORDINATES = {
    0.0f, 0.0f, 0.0f, // 0
    1.0f, 0.0f, 0.0f, // 1
    1.0f, 1.0f, 0.0f, // 2
    0.0f, 1.0f, 0.0f, // 3

    0.0f, 0.0f, 1.0f, // 4
    1.0f, 0.0f, 1.0f, // 5
    1.0f, 1.0f, 1.0f, // 6
    0.0f, 1.0f, 1.0f, // 7
};

constexpr std::array<unsigned int, 6> FACE_INDICES = {
    0, 1, 2, 2, 3, 0
};

constexpr std::array<unsigned int, 4> UNIQUE_INDICES = {
    0, 1, 2, 4,
};

constexpr std::array<unsigned int, 6 * 6> CUBE_INDICES = {
    1, 0, 3, 3, 2, 1, // North
    4, 5, 6, 6, 7, 4, // South
    5, 1, 2, 2, 6, 5, // East
    0, 4, 7, 7, 3, 0, // West
    2, 3, 7, 7, 6, 2, // Up
    0, 1, 5, 5, 4, 0, // Down
};

constexpr std::array<float, 4 * 2> CUBE_UVS = {
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
};

void BlockMeshParams::AppendFace(ChunkMesh& target) {
    // Emit vertices
    for (int i = 0; i < 4; i++) {
        const float* coords = &CUBE_COORDINATES[CUBE_INDICES[(direction * 6) + UNIQUE_INDICES[i]] * 3];
        const float* uv = &CUBE_UVS[i * 2];
        
        glm::vec3 vertex_position = {
            position.x + coords[0],
            position.y + coords[1],
            position.z + coords[2],
        };

        glm::vec2 vertex_tex_coords = {
            uv[0] == 1.0f ? uv_max.x : uv_min.x,
            uv[1] == 1.0f ? uv_max.y : uv_min.y,
        };

        target.m_vertices.push_back({ vertex_position, vertex_tex_coords });
    }

    // Emit indices
    for (unsigned int i = 0; i < 6; i++) {
        target.m_indices.push_back({ target.m_vertex_count + FACE_INDICES[i] });
    }

    target.m_vertex_count += 4;
    target.m_index_count += 6;
}
