#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <functional>
#include <cmath>

#include "chunk.hpp"
#include "player.hpp"
#include "../common/types.hpp"

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

    [[nodiscard]]
    auto is_created() const -> bool;

    void update(f32 dt);
    void prepare_render();
    void render() const;

    void set_center(const glm::ivec3& position);

    [[nodiscard]]
    auto chunk_in_bounds(const glm::ivec3& offset) const -> bool;

    [[nodiscard]]
    auto block_in_bounds(const glm::ivec3& position) const -> bool;

    [[nodiscard]]
    auto contains_chunk(const glm::ivec3& offset) const -> bool;

    [[nodiscard]]
    auto contains(const glm::ivec3& position) const -> bool;

    [[nodiscard]]
    auto chunk_index(const glm::ivec3& offset) const -> size_t;

    [[nodiscard]]
    auto chunk_offset(size_t index) const -> glm::ivec3;

    [[nodiscard]]
    auto get_block(const glm::ivec3& position) const -> Block;
    void set_block(const glm::ivec3& position, Block block);

    [[nodiscard]]
    auto get_chunk(const glm::ivec3& offset) const -> const Chunk&;
    auto get_chunk(const glm::ivec3& offset) -> Chunk&;

    [[nodiscard]]
    auto get_seed() const -> u64;

    [[nodiscard]]
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

    u64 m_seed;

    void create_missing_chunks();
};

// world position (float) -> block position
inline auto position_to_block(const glm::vec3& position) -> glm::ivec3 {
    return static_cast<glm::ivec3>(glm::floor(position));
}

// world position -> chunk offset
inline auto block_to_offset(const glm::ivec3& position) -> glm::ivec3 {
    return glm::ivec3 {
        static_cast<i32>(floorf(static_cast<f32>(position.x) / CHUNK_SIZE<glm::vec3>.x)),
        0,
        static_cast<i32>(floorf(static_cast<f32>(position.z) / CHUNK_SIZE<glm::vec3>.z)),
    };
}

// world position -> chunk position
constexpr auto block_to_chunk(const glm::ivec3& position) -> glm::ivec3 {
    return ((position % CHUNK_SIZE<>) + CHUNK_SIZE<>) % CHUNK_SIZE<>;
}
