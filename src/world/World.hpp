#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <functional>

#include "Chunk.hpp"
#include "Player.hpp"

template<typename T = glm::ivec3>
constexpr T world_size = T(16, 0, 16);

constexpr int world_area =
    world_size<>.x *
    world_size<>.z;

constexpr auto world_offset = glm::ivec3 {
    world_size<>.x / 2,
    0,
    world_size<>.z / 2
};

class World {
public:
    void Create();
    void Destroy();

    auto IsCreated() const -> bool;

    void Update(float dt);
    void PrepareRender();
    void Render() const;

    void SetCenter(const glm::ivec3& position);

    auto ChunkInBounds(const glm::ivec3& offset) const -> bool;
    auto BlockInBounds(const glm::ivec3& position) const -> bool;

    auto ContainsChunk(const glm::ivec3& offset) const -> bool;
    auto Contains(const glm::ivec3& position) const -> bool;

    auto ChunkIndex(const glm::ivec3& offset) const -> size_t;
    auto ChunkOffset(const size_t index) const -> glm::ivec3;

    auto GetBlock(const glm::ivec3& position) const -> Block;
    void SetBlock(const glm::ivec3& position, Block block);

    auto GetChunk(const glm::ivec3& offset) const -> const Chunk&;
    auto GetChunk(const glm::ivec3& offset) -> Chunk&;

private:
    std::vector<Chunk> m_chunks;
    glm::ivec3 m_center {};

    Player m_player;

    void CreateMissingChunks();
};

// world position (float) -> block position
constexpr auto PositionToBlock(const glm::vec3& position) -> glm::ivec3 {
    return (glm::ivec3)glm::floor(position);
}

// world position -> chunk offset
constexpr auto BlockToOffset(const glm::ivec3& position) -> glm::ivec3 {
    return glm::ivec3 {
        (int)std::floorf((float)position.x / chunk_size<glm::vec3>.x),
        (int)0,
        (int)std::floorf((float)position.z / chunk_size<glm::vec3>.z),
    };
}

// world position -> chunk position
constexpr auto BlockToChunk(const glm::ivec3& position) -> glm::ivec3 {
    return ((position % chunk_size<>) + chunk_size<>) % chunk_size<>;
}
