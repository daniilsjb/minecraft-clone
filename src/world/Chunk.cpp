#include "Chunk.hpp"
#include "../State.hpp"
#include "../gfx/Renderer.hpp"
#include "World.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void Chunk::Create(World* world, const glm::ivec3& offset) {
    m_blocks.resize(chunk_volume);
    m_mesh.Create();

    // Temporary world generation
    std::fill(m_blocks.begin(), m_blocks.end(), Block { BLOCK_GRASS });

    m_flags.dirty = true;

    m_world = world;
    m_offset = offset;
    m_position = offset * chunk_size<>;
}

void Chunk::Destroy() {
    m_mesh.Destroy();
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
}

void Chunk::PrepareRender() {
    if (m_flags.dirty) {
        m_mesh.Mesh(*this);
        m_flags.dirty = false;
    }
}

void Chunk::Render() const {
    glm::mat4 model = glm::translate(glm::mat4(1.0f), (glm::vec3)m_position);
    State::renderer->shaders[SHADER_CHUNK].SetUniform("u_model", model);

    m_mesh.Render();
}

auto Chunk::GetBlock(const glm::ivec3& position) const -> Block {
    return m_blocks[ChunkPositionToIndex(position)];
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
