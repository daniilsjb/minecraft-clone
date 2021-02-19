#pragma once

#include <glm/glm.hpp>

#include <functional>
#include <vector>

#include "Block.hpp"
#include "ChunkMesh.hpp"

template<typename T = glm::ivec3>
constexpr T chunk_size = T(16, 256, 16);

constexpr int chunk_volume =
    chunk_size<>.x *
    chunk_size<>.y *
    chunk_size<>.z;

using BlockFunction = std::function<void(const glm::ivec3& position, Block block)>;

class World;

class Chunk {
public:
    void Create(World* world, const glm::ivec3& location);
    void Destroy();

    auto IsCreated() const -> bool;

    void ForEach(const BlockFunction& function) const;

    void Update();
    void PrepareRender();
    void Render() const;

    auto GetBlock(const glm::ivec3& position) const -> Block;
    void SetBlock(const glm::ivec3& position, Block block);

    auto GetWorld() const -> World*;
    auto GetOffset() const -> glm::ivec3;
    auto GetPosition() const -> glm::ivec3;

private:
    std::vector<Block> m_blocks;
    ChunkMesh m_mesh;

    // Absolute position in the chunk coordinate system (in terms of chunks)
    glm::ivec3 m_offset;

    // Absolute position in the global coordinate system (in terms of unit blocks)
    glm::ivec3 m_position;

    World* m_world;

    struct {
        // If true, the mesh has been modified and its mesh must be re-generated
        bool dirty : 1;
    } m_flags {};

    void GetBorderingChunks(const glm::ivec3& position, Chunk* dest[2]);
};

constexpr auto PositionInChunkBounds(const glm::ivec3& position) -> bool {
    return (position.x >= 0 && position.x < chunk_size<>.x) &&
           (position.y >= 0 && position.y < chunk_size<>.y) &&
           (position.z >= 0 && position.z < chunk_size<>.z);
}

constexpr auto ChunkPositionToIndex(const glm::ivec3& position) -> int {
    return position.x * chunk_size<>.y * chunk_size<>.z +
           position.y * chunk_size<>.z +
           position.z;
}
