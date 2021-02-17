#include "World.hpp"
#include "../State.hpp"
#include "../gfx/Renderer.hpp"

void World::Create() {
    m_center = { 0, 0, 0 };
    m_chunks.resize(world_area);
    CreateMissingChunks();

    m_player.Init();
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
        chunk.Render();
    }
}

auto World::ChunkInBounds(const glm::ivec3& offset) const -> bool {
    const glm::ivec3 p = offset - m_center;
    return (p.x >= 0 && p.x < world_size<>.x) && 
           (p.y == 0) &&
           (p.z >= 0 && p.z < world_size<>.z);
}

auto World::BlockInBounds(const glm::ivec3& position) const -> bool {
    return ChunkInBounds(BlockToOffset(position));
}

auto World::ChunkIndex(const glm::ivec3& offset) const -> size_t {
    const glm::ivec3 p = offset - m_center;
    return p.z * chunk_size<>.x + p.x;
}

auto World::ChunkOffset(const size_t index) const -> glm::ivec3 {
    return m_center + glm::ivec3 {
        index % chunk_size<>.x, 0, index / chunk_size<>.z
    };
}

auto World::GetChunk(const glm::ivec3& offset) const -> const Chunk* {
    if (!ChunkInBounds(offset)) {
        return nullptr;
    }

    return &m_chunks[ChunkIndex(offset)];
}

void World::CreateMissingChunks() {
    for (size_t i = 0; i < world_area; i++) {
        Chunk& chunk = m_chunks[i];

        if (!chunk.IsCreated()) {
            chunk.Create(this, ChunkOffset(i));
        }
    }
}
