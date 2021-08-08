#pragma once

#include "block.hpp"
#include "chunk_mesh.hpp"

template<typename T = glm::ivec3>
constexpr T CHUNK_SIZE = T(16, 256, 16);

constexpr i32 CHUNK_VOLUME =
    CHUNK_SIZE<>.x *
    CHUNK_SIZE<>.y *
    CHUNK_SIZE<>.z;

using BlockFunction = std::function<void(const glm::ivec3& position, Block block)>;

class World;

class Chunk {
public:
    void create(World* world, const glm::ivec3& location);
    void destroy();

    [[nodiscard]]
    auto is_created() const -> bool;

    void for_each(const BlockFunction& function) const;

    void update();
    void prepare_render();
    void render(bool transparent) const;

    [[nodiscard]]
    auto get_block(const glm::ivec3& position) const -> Block;
    void set_block(const glm::ivec3& position, Block block);

    [[nodiscard]]
    auto get_world() const -> World*;

    [[nodiscard]]
    auto get_offset() const -> glm::ivec3;

    [[nodiscard]]
    auto get_position() const -> glm::ivec3;

    void set_dirty();
    void set_sort();

private:
    std::vector<Block> m_blocks;
    ChunkMesh m_opaque_mesh;
    ChunkMesh m_transparent_mesh;

    // Absolute position in the chunk coordinate system (in terms of chunks)
    glm::ivec3 m_offset;

    // Absolute position in the global coordinate system (in terms of unit blocks)
    glm::ivec3 m_position;

    World* m_world;

    struct {
        // If true, the mesh has been modified and its mesh must be re-generated
        bool dirty : 1;

        // If true, the transparent block faces need to be re-sorted
        bool sort : 1;
    } m_flags {};

    void get_bordering_chunks(const glm::ivec3& position, Chunk* dest[2]);
};

constexpr auto position_in_chunk_bounds(const glm::ivec3& position) -> bool {
    return (position.x >= 0 && position.x < CHUNK_SIZE<>.x) &&
           (position.y >= 0 && position.y < CHUNK_SIZE<>.y) &&
           (position.z >= 0 && position.z < CHUNK_SIZE<>.z);
}

constexpr auto chunk_position_to_index(const glm::ivec3& position) -> int {
    return position.x * CHUNK_SIZE<>.y * CHUNK_SIZE<>.z +
           position.y * CHUNK_SIZE<>.z +
           position.z;
}
