#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <functional>
#include <optional>

#include "Chunk.hpp"
#include "Player.hpp"

template<typename T = glm::ivec3>
constexpr T world_size = T(16, 0, 16);

constexpr int world_area =
    world_size<>.x *
    world_size<>.z;

template<typename T = glm::ivec3>
constexpr T world_offset = world_size<T> / T(2);

class World {
public:
    void Create();
    void Destroy();

    auto IsCreated() const -> bool;

    void Update(float dt);
    void PrepareRender();
    void Render() const;

    auto ChunkInBounds(const glm::ivec3& offset) const -> bool;
    auto BlockInBounds(const glm::ivec3& position) const -> bool;

    auto ChunkIndex(const glm::ivec3& offset) const -> size_t;
    auto ChunkOffset(const size_t index) const -> glm::ivec3;
    auto GetChunk(const glm::ivec3& offset) const -> const Chunk*;

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
    return (glm::ivec3)glm::floor((glm::vec3)position / chunk_size<glm::vec3>);
}

// world position -> chunk position
constexpr auto BlockToChunk(const glm::ivec3& position) -> glm::ivec3 {
    return ((position % chunk_size<>) + chunk_size<>) % chunk_size<>;
}
