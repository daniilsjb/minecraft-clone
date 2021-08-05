#include "Chunk.hpp"
#include "../State.hpp"
#include "../gfx/Renderer.hpp"
#include "World.hpp"
#include "Generator.hpp"

void Chunk::Create(World* world, const glm::ivec3& offset) {
    m_blocks.resize(chunk_volume);
    m_opaque_mesh.Create();
    m_transparent_mesh.Create();

    m_world = world;
    m_offset = offset;
    m_position = offset * chunk_size<>;

    Generate(*this, m_world->GetSeed());
}

void Chunk::Destroy() {
    m_opaque_mesh.Destroy();
    m_transparent_mesh.Destroy();
    m_blocks.clear();

    m_flags = {};
}

auto Chunk::IsCreated() const -> bool {
    return !m_blocks.empty();
}

void Chunk::ForEach(const BlockFunction& function) const {
    glm::ivec3 position = { 0, 0, 0 };
    for (position.x = 0; position.x < chunk_size<>.x; position.x++) {
        for (position.z = 0; position.z < chunk_size<>.z; position.z++) {
            for (position.y = 0; position.y < chunk_size<>.y; position.y++) {
                function(position, GetBlock(position));
            }
        }
    }
}

void Chunk::Update() {
    Player* player = State::world->GetPlayer();
    m_flags.sort = (player->changed_block && glm::distance(static_cast<glm::vec3>(player->offset),static_cast<glm::vec3>(m_offset)) < 2.0f);
}

void Chunk::PrepareRender() {
    if (m_flags.dirty) {
        m_opaque_mesh.Mesh(*this, false);
        m_transparent_mesh.Mesh(*this, true);
        m_flags.dirty = false;
        m_flags.sort = false;
    }

    if (m_flags.sort) {
        m_transparent_mesh.Sort();
        m_flags.sort = false;
    }
}

void Chunk::Render(bool transparent) const {
    glm::mat4 model = glm::translate(glm::mat4(1.0f), static_cast<glm::vec3>(m_position));
    State::renderer->shaders[SHADER_CHUNK].SetUniform("u_model", model);

    if (transparent) {
        m_transparent_mesh.Render();
    } else {
        m_opaque_mesh.Render();
    }
}

auto Chunk::GetBlock(const glm::ivec3& position) const -> Block {
    return m_blocks[static_cast<size_t>(ChunkPositionToIndex(position))];
}

void Chunk::SetBlock(const glm::ivec3& position, Block block) {
    m_blocks[static_cast<size_t>(ChunkPositionToIndex(position))] = block;
    m_flags.dirty = true;

    Chunk* chunks[2] = { nullptr, nullptr };
    GetBorderingChunks(position, chunks);

    if (chunks[0] != nullptr) {
        chunks[0]->m_flags.dirty = true;
    }
    if (chunks[1] != nullptr) {
        chunks[1]->m_flags.dirty = true;
    }
}

auto Chunk::GetWorld() const -> World* {
    return m_world;
}

auto Chunk::GetOffset() const -> glm::ivec3 {
    return m_offset;
}

auto Chunk::GetPosition() const -> glm::ivec3 {
    return m_position;
}

void Chunk::SetDirty() {
    m_flags.dirty = true;
}

void Chunk::SetSort() {
    m_flags.sort = true;
}

void Chunk::GetBorderingChunks(const glm::ivec3& position, Chunk* dest[2]) {
    size_t current = 0;

    if (position.x == 0) {
        glm::ivec3 offset = m_offset + glm::ivec3 { -1, 0, 0 };
        if (m_world->ContainsChunk(offset)) {
            dest[current++] = &m_world->GetChunk(offset);
        }
    }
    if (position.x == chunk_size<>.x - 1) {
        glm::ivec3 offset = m_offset + glm::ivec3 { 1, 0, 0 };
        if (m_world->ContainsChunk(offset)) {
            dest[current++] = &m_world->GetChunk(offset);
        }
    }
    if (position.z == 0) {
        glm::ivec3 offset = m_offset + glm::ivec3 { 0, 0, -1 };
        if (m_world->ContainsChunk(offset)) {
            dest[current++] = &m_world->GetChunk(offset);
        }
    }
    if (position.z == chunk_size<>.z - 1) {
        glm::ivec3 offset = m_offset + glm::ivec3 { 0, 0, 1 };
        if (m_world->ContainsChunk(offset)) {
            dest[current++] = &m_world->GetChunk(offset);
        }
    }
}
