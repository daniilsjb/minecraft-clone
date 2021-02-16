#pragma once

#include <glm/glm.hpp>

#include <functional>
#include <vector>

#include "Block.hpp"
#include "ChunkMesh.hpp"

constexpr glm::ivec3 CHUNK_SIZE = { 16, 2, 16 };
constexpr size_t CHUNK_VOLUME = CHUNK_SIZE.x * CHUNK_SIZE.y * CHUNK_SIZE.z;

using BlockFunction = std::function<void(const glm::ivec3& pos, Block block)>;

class World;

class Chunk {
public:
    World* world { nullptr };

    glm::ivec3 location { 0, 0, 0 };
    glm::ivec3 position { 0, 0, 0 };

    Chunk() = default;
    Chunk(Chunk&& other) noexcept;

    void Init(World* world);
    void Update();
    void Render();

    bool IsEmpty() const;
    void Generate(const glm::ivec3& location);

    void ForEach(const BlockFunction& fun) const;

    bool IsWithinBounds(int x, int y, int z) const;
    bool IsWithinBounds(const glm::ivec3& pos) const;

    Block GetBlock(int x, int y, int z) const;
    Block GetBlock(const glm::ivec3& pos) const;

private:
    std::vector<Block> m_blocks;
    ChunkMesh m_mesh;

    bool m_isEmpty { true };
};