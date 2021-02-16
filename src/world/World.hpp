#pragma once

#include <array>
#include <functional>

#include "../general/Mixins.hpp"
#include "Chunk.hpp"
#include "Player.hpp"

// The world's dimensions, in chunks, being positioned around the player
constexpr int WORLD_SIZE_X = 20;
constexpr int WORLD_SIZE_Z = 20;

// The total number of chunks in the world
constexpr int WORLD_AREA = WORLD_SIZE_X * WORLD_SIZE_Z;

using ChunkFunction = std::function<void(const glm::ivec3& location, Chunk& chunk)>;

class World : private NonCopyable, private NonMoveable {
public:
    Player player;
    Chunk** chunks { nullptr };

    glm::ivec3 center { 0, 0, 0 };
    glm::ivec3 offset { WORLD_SIZE_X / 2, 0, WORLD_SIZE_Z / 2 };

    bool Init();
    void Update(float dt);
    void Render();

    glm::ivec3 BlockToChunk(const glm::ivec3& pos) const;
    glm::ivec3 ChunkToBlock(const glm::ivec3& pos) const;
    glm::ivec3 ChunkToIndex(const glm::ivec3& pos) const;

    bool IsWithinBounds(const glm::ivec3& location) const;
    bool IsSolidBlock(const glm::ivec3& pos) const;

    void ForEach(const ChunkFunction& fun);

    void SetCenter(const glm::ivec3& pos);

private:
    void LoadEmptyChunks();
};
