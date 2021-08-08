#include "world.hpp"
#include "../state.hpp"
#include "../gfx/renderer.hpp"
#include "../gfx/window.hpp"

#include <ctime>

void World::create() {
    m_player.init();
    m_seed = static_cast<u64>(time(nullptr));

    m_chunks.resize(WORLD_AREA);
    set_center(State::renderer->camera.position);
}

void World::destroy() {
    m_player.destroy();

    m_chunks.clear();
    m_center = { 0, 0, 0 };
}

auto World::is_created() const -> bool {
    return !m_chunks.empty();
}

void World::update(f32 dt) {
    m_player.update(dt);

    for (auto& chunk : m_chunks) {
        chunk.update();
    }
}

void World::prepare_render() {
    for (auto& chunk : m_chunks) {
        chunk.prepare_render();
    }
}

void World::render() const {
    State::renderer->shaders[SHADER_CHUNK].bind();
    State::renderer->shaders[SHADER_CHUNK].set_uniform("u_projection", State::renderer->camera.projection);
    State::renderer->shaders[SHADER_CHUNK].set_uniform("u_view", State::renderer->camera.view);
    State::renderer->shaders[SHADER_CHUNK].set_uniform("u_atlas", 0);

    State::renderer->atlas.bind();

    for (const auto& chunk : m_chunks) {
        chunk.render(false);
    }

    // Draw chunks in the sorted order, from farthest to the nearest
    struct ChunkSort {
        glm::ivec3 offset;
        f32 distance;
    };

    // Calculate offset and distance to each visible chunk
    std::array<ChunkSort, WORLD_AREA> sorted;
    for (size_t i = 0; i < sorted.size(); i++) {
        glm::ivec3 offset = m_chunks[i].get_offset();
        f32 distance = glm::distance(
            static_cast<glm::vec3>(offset),
            static_cast<glm::vec3>(m_offset)
        );

        sorted[i] = { offset, distance };
    }

    // Sort them by distance
    std::sort(sorted.begin(), sorted.end(), [](const ChunkSort& a, const ChunkSort& b) {
        return a.distance > b.distance;
    });

    // Render them by reconstructing indices from sorted offsets
    glDisable(GL_CULL_FACE);
    for (const auto& chunk : sorted) {
        m_chunks[chunk_index(chunk.offset)].render(true);
    }
    glEnable(GL_CULL_FACE);
}

void World::set_center(const glm::ivec3& position) {
    // Find the chunk containing new world center
    glm::ivec3 new_offset = block_to_offset(position);
    glm::ivec3 new_center = new_offset - WORLD_OFFSET;

    // If didn't move, nothing needs to change
    if (m_center == new_center) {
        return;
    }
    m_offset = new_offset;
    m_center = new_center;

    // Move old chunks into another vector and set up a new collection of chunks
    std::vector<Chunk> old;
    m_chunks.swap(old);
    m_chunks.resize(WORLD_AREA);

    // Move old chunks back into the new set if they are still in bounds
    for (auto& chunk : old) {
        if (chunk.is_created() && chunk_in_bounds(chunk.get_offset())) {
            m_chunks[chunk_index(chunk.get_offset())] = std::move(chunk);
        }
    }

    // Everything else needs to be created manually
    create_missing_chunks();
}

auto World::chunk_in_bounds(const glm::ivec3& offset) const -> bool {
    const auto p = offset - m_center;
    return (p.x >= 0 && p.x < WORLD_SIZE<>.x) &&
           (p.z >= 0 && p.z < WORLD_SIZE<>.z);
}

auto World::block_in_bounds(const glm::ivec3& position) const -> bool {
    return chunk_in_bounds(block_to_offset(position));
}

auto World::contains_chunk(const glm::ivec3& offset) const -> bool {
    if (!chunk_in_bounds(offset)) {
        return false;
    }

    return m_chunks[chunk_index(offset)].is_created();
}

auto World::contains(const glm::ivec3& position) const -> bool {
    return contains_chunk(block_to_offset(position));
}

auto World::chunk_index(const glm::ivec3& offset) const -> size_t {
    const auto p = offset - m_center;
    return static_cast<size_t>(p.z) * static_cast<size_t>(CHUNK_SIZE<>.x) + static_cast<size_t>(p.x);
}

auto World::chunk_offset(const size_t index) const -> glm::ivec3 {
    return m_center + glm::ivec3 {
        index % CHUNK_SIZE<>.x, 0, index / CHUNK_SIZE<>.z
    };
}

auto World::get_block(const glm::ivec3& position) const -> Block {
    const auto offset = block_to_offset(position);
    if (position.y >= 0 && position.y < CHUNK_SIZE<>.y && contains_chunk(offset)) {
        return get_chunk(offset).get_block(block_to_chunk(position));
    }
    return { BLOCK_AIR };
}

void World::set_block(const glm::ivec3& position, Block block) {
    const auto offset = block_to_offset(position);
    if (contains_chunk(offset)) {
        m_chunks[chunk_index(offset)].set_block(block_to_chunk(position), block);
    } else {
        m_queued_blocks.push_back({ position, block });
    }
}

auto World::get_chunk(const glm::ivec3& offset) const -> const Chunk& {
    return m_chunks[chunk_index(offset)];
}

auto World::get_chunk(const glm::ivec3& offset) -> Chunk& {
    return m_chunks[chunk_index(offset)];
}

auto World::get_seed() const -> u64 {
    return m_seed;
}

auto World::get_player() -> Player* {
    return &m_player;
}

void World::create_missing_chunks() {
    for (size_t i = 0; i < WORLD_AREA; i++) {
        Chunk& chunk = m_chunks[i];

        if (!chunk.is_created()) {
            chunk.create(this, chunk_offset(i));
        }
    }

    for (auto it = m_queued_blocks.begin(); it != m_queued_blocks.end();) {
        if (contains(it->position)) {
            set_block(it->position, it->block);
            it = m_queued_blocks.erase(it);
        } else {
            it++;
        }
    }
}
