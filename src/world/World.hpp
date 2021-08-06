#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <functional>
#include <cstdint>
#include <cmath>

#include "Chunk.hpp"
#include "Player.hpp"

template<typename T = glm::ivec3>
constexpr T WORLD_SIZE = T(16, 0, 16);

constexpr int WORLD_AREA =
    WORLD_SIZE<>.x *
    WORLD_SIZE<>.z;

constexpr auto WORLD_OFFSET = glm::ivec3 {
    WORLD_SIZE<>.x / 2,
    0,
    WORLD_SIZE<>.z / 2
};

class World {
public:
    void create();
    void destroy();

    auto is_created() const -> bool;

    void update(float dt);
    void prepare_render();
    void render() const;

    void set_center(const glm::ivec3& position);

    auto chunk_in_bounds(const glm::ivec3& offset) const -> bool;
    auto block_in_bounds(const glm::ivec3& position) const -> bool;

    auto contains_chunk(const glm::ivec3& offset) const -> bool;
    auto contains(const glm::ivec3& position) const -> bool;

    auto chunk_index(const glm::ivec3& offset) const -> size_t;
    auto chunk_offset(const size_t index) const -> glm::ivec3;

    auto get_block(const glm::ivec3& position) const -> Block;
    void set_block(const glm::ivec3& position, Block block);

    auto get_chunk(const glm::ivec3& offset) const -> const Chunk&;
    auto get_chunk(const glm::ivec3& offset) -> Chunk&;

    auto get_seed() const -> uint64_t;
    auto get_player() -> Player*;

private:
    std::vector<Chunk> m_chunks;
    glm::ivec3 m_center, m_offset {};

    // Blocks that were placed in the world before their containing chunks were loaded
    struct QueuedBlock {
        glm::ivec3 position;
        Block block;
    };

    std::vector<QueuedBlock> m_queued_blocks;

    Player m_player;

    uint64_t m_seed;

    void create_missing_chunks();
};

// world position (float) -> block position
inline auto position_to_block(const glm::vec3& position) -> glm::ivec3 {
    return static_cast<glm::ivec3>(glm::floor(position));
}

// world position -> chunk offset
inline auto block_to_offset(const glm::ivec3& position) -> glm::ivec3 {
    return glm::ivec3 {
        static_cast<int>(floorf(static_cast<float>(position.x) / CHUNK_SIZE<glm::vec3>.x)),
        0,
        static_cast<int>(floorf(static_cast<float>(position.z) / CHUNK_SIZE<glm::vec3>.z)),
    };
}

// world position -> chunk position
constexpr auto block_to_chunk(const glm::ivec3& position) -> glm::ivec3 {
    return ((position % CHUNK_SIZE<>) + CHUNK_SIZE<>) % CHUNK_SIZE<>;
}
