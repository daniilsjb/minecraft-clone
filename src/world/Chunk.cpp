#include "Chunk.hpp"
#include "../State.hpp"
#include "../gfx/Renderer.hpp"
#include "World.hpp"
#include "Generator.hpp"

void Chunk::create(World* world, const glm::ivec3& offset) {
    m_blocks.resize(CHUNK_VOLUME);
    m_opaque_mesh.create();
    m_transparent_mesh.create();

    m_world = world;
    m_offset = offset;
    m_position = offset * CHUNK_SIZE<>;

    generate(*this, m_world->get_seed());
}

void Chunk::destroy() {
    m_opaque_mesh.destroy();
    m_transparent_mesh.destroy();
    m_blocks.clear();

    m_flags = {};
}

auto Chunk::is_created() const -> bool {
    return !m_blocks.empty();
}

void Chunk::for_each(const BlockFunction& function) const {
    glm::ivec3 position = { 0, 0, 0 };
    for (position.x = 0; position.x < CHUNK_SIZE<>.x; position.x++) {
        for (position.z = 0; position.z < CHUNK_SIZE<>.z; position.z++) {
            for (position.y = 0; position.y < CHUNK_SIZE<>.y; position.y++) {
                function(position, get_block(position));
            }
        }
    }
}

void Chunk::update() {
    Player* player = State::world->get_player();
    m_flags.sort = (player->changed_block && glm::distance(static_cast<glm::vec3>(player->offset),static_cast<glm::vec3>(m_offset)) < 2.0f);
}

void Chunk::prepare_render() {
    if (m_flags.dirty) {
        m_opaque_mesh.mesh(*this, false);
        m_transparent_mesh.mesh(*this, true);
        m_flags.dirty = false;
        m_flags.sort = false;
    }

    if (m_flags.sort) {
        m_transparent_mesh.sort();
        m_flags.sort = false;
    }
}

void Chunk::render(bool transparent) const {
    auto model = glm::translate(glm::mat4(1.0f), static_cast<glm::vec3>(m_position));
    State::renderer->shaders[SHADER_CHUNK].set_uniform("u_model", model);

    if (transparent) {
        m_transparent_mesh.render();
    } else {
        m_opaque_mesh.render();
    }
}

auto Chunk::get_block(const glm::ivec3& position) const -> Block {
    return m_blocks[static_cast<size_t>(chunk_position_to_index(position))];
}

void Chunk::set_block(const glm::ivec3& position, Block block) {
    m_blocks[static_cast<size_t>(chunk_position_to_index(position))] = block;
    m_flags.dirty = true;

    Chunk* chunks[2] = { nullptr, nullptr };
    get_bordering_chunks(position, chunks);

    if (chunks[0] != nullptr) {
        chunks[0]->m_flags.dirty = true;
    }
    if (chunks[1] != nullptr) {
        chunks[1]->m_flags.dirty = true;
    }
}

auto Chunk::get_world() const -> World* {
    return m_world;
}

auto Chunk::get_offset() const -> glm::ivec3 {
    return m_offset;
}

auto Chunk::get_position() const -> glm::ivec3 {
    return m_position;
}

void Chunk::set_dirty() {
    m_flags.dirty = true;
}

void Chunk::set_sort() {
    m_flags.sort = true;
}

void Chunk::get_bordering_chunks(const glm::ivec3& position, Chunk* dest[2]) {
    size_t current = 0;

    if (position.x == 0) {
        glm::ivec3 offset = m_offset + glm::ivec3 { -1, 0, 0 };
        if (m_world->contains_chunk(offset)) {
            dest[current++] = &m_world->get_chunk(offset);
        }
    }
    if (position.x == CHUNK_SIZE<>.x - 1) {
        glm::ivec3 offset = m_offset + glm::ivec3 { 1, 0, 0 };
        if (m_world->contains_chunk(offset)) {
            dest[current++] = &m_world->get_chunk(offset);
        }
    }
    if (position.z == 0) {
        glm::ivec3 offset = m_offset + glm::ivec3 { 0, 0, -1 };
        if (m_world->contains_chunk(offset)) {
            dest[current++] = &m_world->get_chunk(offset);
        }
    }
    if (position.z == CHUNK_SIZE<>.z - 1) {
        glm::ivec3 offset = m_offset + glm::ivec3 { 0, 0, 1 };
        if (m_world->contains_chunk(offset)) {
            dest[current++] = &m_world->get_chunk(offset);
        }
    }
}
