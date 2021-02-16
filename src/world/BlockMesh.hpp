#pragma once

#include "Block.hpp"

class ChunkMesh;

struct BlockMesh {
    glm::vec3 pos;
    Direction dir;

    glm::vec2 uvMin;
    glm::vec2 uvMax;

	void AppendFace(ChunkMesh* target);
};
