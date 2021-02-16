#include "World.hpp"
#include "../State.hpp"
#include "../gfx/Renderer.hpp"

bool World::Init() {
    chunks = new Chunk*[WORLD_AREA];

    ForEach([this](const glm::ivec3& location, Chunk& chunk) {
        chunk.Init(this);
        chunk.Generate(location);
    });

    return true;
}

void World::Update(float dt) {
    player.Update(dt);
}

void World::Render() {
    State::renderer->shaders[SHADER_CHUNK].Bind();
    State::renderer->shaders[SHADER_CHUNK].SetUniform("u_projection", State::renderer->camera.projection);
    State::renderer->shaders[SHADER_CHUNK].SetUniform("u_view", State::renderer->camera.view);
    State::renderer->shaders[SHADER_CHUNK].SetUniform("u_atlas", 0);

    State::renderer->atlas.Bind();

    for (auto& chunk : chunks) {
        chunk.Render();
    }
}

glm::ivec3 World::BlockToChunk(const glm::ivec3& pos) const {
    return (glm::ivec3)glm::floor(((glm::vec3)pos / (glm::vec3)CHUNK_SIZE));
}

glm::ivec3 World::ChunkToBlock(const glm::ivec3& pos) const {
    return pos * CHUNK_SIZE;
}

glm::ivec3 World::ChunkToIndex(const glm::ivec3& pos) const {
    return pos - center + offset;
}

bool World::IsWithinBounds(const glm::ivec3& location) const {
    return (location.x >= 0 && location.x < WORLD_SIZE_X) && (location.z >= 0 && location.z < WORLD_SIZE_Z);
}

bool World::IsSolidBlock(const glm::ivec3& pos) const {
    glm::ivec3 location = BlockToChunk(pos);
    glm::ivec3 index = ChunkToIndex(location);

    if (!IsWithinBounds(index)) {
        return false;
    }

    glm::ivec3 block = {
        pos.x % CHUNK_SIZE.x,
        pos.y,
        pos.z % CHUNK_SIZE.z
    };

    const Chunk& chunk = chunks[location.z * WORLD_SIZE_X + location.x];
    return chunk.IsWithinBounds(block) && chunk.GetBlock(block).id != BLOCK_AIR;
}

void World::ForEach(const ChunkFunction& fun) {
    for (int x = 0; x < WORLD_SIZE_X; x++) {
        for (int z = 0; z < WORLD_SIZE_Z; z++) {
            fun({ x, 0, z }, chunks[z * WORLD_SIZE_X + x]);
        }
    }
}

void World::SetCenter(const glm::ivec3& pos) {
    // Calculate the new center chunk position
    glm::ivec3 newCenter = BlockToChunk(pos);

    // Only continue if the center chunk has changed
    if (newCenter == center) {
        return;
    }

    // Put the world center into a new position
    center = newCenter;

    // Now go through each chunk, calculate the difference between its location and the center, and determine if it should
    // still be within the grid
    for (auto& chunk : chunks) {
        // Calculate the location of the chunk within the world's bounds
        glm::ivec3 location = ChunkToIndex(chunk.location);
        if (IsWithinBounds(location)) {
            // Move the chunk to another cell in the grid, since everything has moved now
        } else {
            // The chunk can simply be removed since it is out of bounds now
        }
    }

    // Load the newly moved chunks
    LoadEmptyChunks();
}

void World::LoadEmptyChunks() {
    ForEach([this](const glm::ivec3& location, Chunk& chunk) {
        if (!chunk.IsEmpty()) {
            return;
        }

        chunk.Generate(this->center + location);
    });
}
