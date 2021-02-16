#include "BlockMesh.hpp"
#include "ChunkMesh.hpp"

#include <array>

static const std::array<float, 8 * 3> CUBE_COORDINATES = {
    0.0f, 0.0f, 0.0f, // 0
    1.0f, 0.0f, 0.0f, // 1
    1.0f, 1.0f, 0.0f, // 2
    0.0f, 1.0f, 0.0f, // 3

    0.0f, 0.0f, 1.0f, // 4
    1.0f, 0.0f, 1.0f, // 5
    1.0f, 1.0f, 1.0f, // 6
    0.0f, 1.0f, 1.0f, // 7
};

static const std::array<unsigned int, 6> FACE_INDICES = {
    0, 1, 2, 2, 3, 0
};

static const std::array<unsigned int, 4> UNIQUE_INDICES = {
    0, 1, 2, 4,
};

static const std::array<unsigned int, 6 * 6> CUBE_INDICES = {
    1, 0, 3, 3, 2, 1, // North
    4, 5, 6, 6, 7, 4, // South
    5, 1, 2, 2, 6, 5, // East
    0, 4, 7, 7, 3, 0, // West
    2, 3, 7, 7, 6, 2, // Up
    0, 1, 5, 5, 4, 0, // Down
};

static const std::array<float, 4 * 2> CUBE_UVS = {
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
};

void BlockMesh::AppendFace(ChunkMesh* target) {
    // Emit vertices
    for (unsigned int i = 0; i < 4; i++) {
        const float* coords = &CUBE_COORDINATES[CUBE_INDICES[(dir * 6) + UNIQUE_INDICES[i]] * 3];
        const float* uv = &CUBE_UVS[i * 2];
        
        glm::vec3 position = {
            pos.x + coords[0],
            pos.y + coords[1],
            pos.z + coords[2],
        };

        glm::vec2 texCoords = {
            uv[0] == 1.0f ? uvMax.x : uvMin.x,
            uv[1] == 1.0f ? uvMax.y : uvMin.y,
        };

        target->vertices.push_back({ position, texCoords });
    }

    // Emit indices
    for (unsigned int i = 0; i < 6; i++) {
        target->indices.push_back({ target->vertexCount + FACE_INDICES[i] });
    }

    target->vertexCount += 4;
    target->indexCount += 6;
}
