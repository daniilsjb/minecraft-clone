#pragma once

#include "../general/Direction.hpp"

class ChunkMesh;

struct BlockMeshParams {
    // Block's position in chunk space
    glm::vec3 position {};

    // Position of the chunk in world space
    glm::vec3 chunk_position {};

    // Direction of the block's current face
    Direction direction {};

    // Coordinate extremes of the face's texture within the block atlas
    glm::vec2 uv_min {};
    glm::vec2 uv_max {};

    // Does the block contain any transparent or translucent pixels?
    bool transparent;

    // Is the block liquid, i.e. should it be shortened on the y axis?
    bool liquid;

	void AppendFace(ChunkMesh& target);
	void AppendSprite(ChunkMesh& target);
};
