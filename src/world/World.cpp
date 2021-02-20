#include "World.hpp"
#include "../State.hpp"
#include "../gfx/Renderer.hpp"
#include "../gfx/Window.hpp"

#include <ctime>
#include <algorithm>

void World::Create() {
    m_player.Init();
    m_seed = (uint64_t)(time(0));

    m_chunks.resize(world_area);
    SetCenter(State::renderer->camera.position);
}

void World::Destroy() {
    m_player.Destroy();

    m_chunks.clear();
    m_center = { 0, 0, 0 };
}

auto World::IsCreated() const -> bool {
    return !m_chunks.empty();
}

void World::Update(float dt) {
    m_player.Update(dt);

    for (auto& chunk : m_chunks) {
        chunk.Update();
    }
}

void World::PrepareRender() {
    for (auto& chunk : m_chunks) {
        chunk.PrepareRender();
    }
}

void World::Render() const {
    State::renderer->shaders[SHADER_CHUNK].Bind();
    State::renderer->shaders[SHADER_CHUNK].SetUniform("u_projection", State::renderer->camera.projection);
    State::renderer->shaders[SHADER_CHUNK].SetUniform("u_view", State::renderer->camera.view);
    State::renderer->shaders[SHADER_CHUNK].SetUniform("u_atlas", 0);

    State::renderer->atlas.Bind();

    for (const auto& chunk : m_chunks) {
        chunk.Render(false);
    }

    // Draw chunks in the sorted order, from farthest to the nearest
    struct ChunkSort {
        glm::ivec3 offset;
        float distance;
    };

    // Calculate offset and distance to each visible chunk
    std::array<ChunkSort, world_area> sorted;
    for (size_t i = 0; i < sorted.size(); i++) {
        glm::ivec3 offset = m_chunks[i].GetOffset();
        float distance = glm::distance((glm::vec3)offset, (glm::vec3)m_offset);

        sorted[i] = { offset, distance };
    }

    // Sort them by distance
    std::sort(sorted.begin(), sorted.end(), [](const ChunkSort& a, const ChunkSort& b) {
        return a.distance > b.distance;
    });

    // Render them by reconstructing indices from sorted offsets
    glDisable(GL_CULL_FACE);
    for (const auto& chunk : sorted) {
        m_chunks[ChunkIndex(chunk.offset)].Render(true);
    }
    glEnable(GL_CULL_FACE);
}

void World::SetCenter(const glm::ivec3& position) {
    // Find the chunk containing new world center
    glm::ivec3 new_offset = BlockToOffset(position);
    glm::ivec3 new_center = new_offset - world_offset;

    // If didn't move, nothing needs to change
    if (m_center == new_center) {
        return;
    }
    m_offset = new_offset;
    m_center = new_center;

    // Move old chunks into another vector and setup a new collection of chunks
    std::vector<Chunk> old;
    m_chunks.swap(old);
    m_chunks.resize(world_area);

    // Move old chunks back into the new set if they are still in bounds
    for (auto& chunk : old) {
        if (chunk.IsCreated() && ChunkInBounds(chunk.GetOffset())) {
            m_chunks[ChunkIndex(chunk.GetOffset())] = std::move(chunk);
        }
    }

    // Everything else needs to be created manually
    CreateMissingChunks();
}

auto World::ChunkInBounds(const glm::ivec3& offset) const -> bool {
    const glm::ivec3 p = offset - m_center;
    return (p.x >= 0 && p.x < world_size<>.x) && 
           (p.z >= 0 && p.z < world_size<>.z);
}

auto World::BlockInBounds(const glm::ivec3& position) const -> bool {
    return ChunkInBounds(BlockToOffset(position));
}

auto World::ContainsChunk(const glm::ivec3& offset) const -> bool {
    if (!ChunkInBounds(offset)) {
        return false;
    }

    return m_chunks[ChunkIndex(offset)].IsCreated();
}

auto World::Contains(const glm::ivec3& position) const -> bool {
    return ContainsChunk(BlockToOffset(position));
}

auto World::ChunkIndex(const glm::ivec3& offset) const -> size_t {
    const glm::ivec3 p = offset - m_center;
    return (size_t)p.z * (size_t)chunk_size<>.x + (size_t)p.x;
}

auto World::ChunkOffset(const size_t index) const -> glm::ivec3 {
    return m_center + glm::ivec3 {
        index % chunk_size<>.x, 0, index / chunk_size<>.z
    };
}

auto World::GetBlock(const glm::ivec3& position) const -> Block {
    const glm::ivec3 offset = BlockToOffset(position);
    if (position.y >= 0 && position.y < chunk_size<>.y && ContainsChunk(offset)) {
        return GetChunk(offset).GetBlock(BlockToChunk(position));
    }
    return { BLOCK_AIR };
}

void World::SetBlock(const glm::ivec3& position, Block block) {
    const glm::ivec3 offset = BlockToOffset(position);
    if (ContainsChunk(offset)) {
        m_chunks[ChunkIndex(offset)].SetBlock(BlockToChunk(position), block);
    } else {
        m_queued_blocks.push_back({ position, block });
    }
}

auto World::GetChunk(const glm::ivec3& offset) const -> const Chunk& {
    return m_chunks[ChunkIndex(offset)];
}

auto World::GetChunk(const glm::ivec3& offset) -> Chunk& {
    return m_chunks[ChunkIndex(offset)];
}

auto World::GetSeed() const -> uint64_t {
    return m_seed;
}

auto World::GetPlayer() -> Player* {
    return &m_player;
}

void World::CreateMissingChunks() {
    for (size_t i = 0; i < world_area; i++) {
        Chunk& chunk = m_chunks[i];

        if (!chunk.IsCreated()) {
            chunk.Create(this, ChunkOffset(i));
        }
    }

    for (auto it = m_queued_blocks.begin(); it != m_queued_blocks.end();) {
        if (Contains(it->position)) {
            SetBlock(it->position, it->block);
            it = m_queued_blocks.erase(it);
        } else {
            it++;
        }
    }
}
