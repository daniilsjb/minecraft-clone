#pragma once

#include "Direction.hpp"

class ChunkMesh;

struct BlockMeshParams {
    // Block's position in chunk space
    glm::vec3 position {};

    // Direction of the block's current face
    Direction direction {};

    // Coordinate extremes of the face's texture within the block atlas
    glm::vec2 uv_min {};
    glm::vec2 uv_max {};

	void AppendFace(ChunkMesh& target);
};
