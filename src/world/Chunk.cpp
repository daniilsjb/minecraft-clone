#include "Chunk.hpp"
#include "../State.hpp"
#include "../gfx/Renderer.hpp"
#include "World.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Chunk::Chunk(Chunk&& other) noexcept
    : m_mesh(std::move(other.m_mesh)) {
    m_blocks.insert(m_blocks.end(),
        std::make_move_iterator(other.m_blocks.begin()),
        std::make_move_iterator(other.m_blocks.end())
    );
}

void Chunk::Init(World* world) {
    this->world = world;
}

void Chunk::Update() {
}

void Chunk::Render() {
    glm::mat4 model = glm::translate(glm::mat4(1.0f), (glm::vec3)position);
    State::renderer->shaders[SHADER_CHUNK].SetUniform("u_model", model);

    m_mesh.Render();
}

bool Chunk::IsEmpty() const {
    return m_isEmpty;
}

void Chunk::Generate(const glm::ivec3& location) {
    this->location = location;
    this->position = location * CHUNK_SIZE;

    // Temporary solution until real world generation
    m_blocks.resize(CHUNK_VOLUME);
    std::fill(m_blocks.begin(), m_blocks.end(), Block { BLOCK_STONE });

    m_mesh.Mesh();

    m_isEmpty = false;
}

void Chunk::ForEach(const BlockFunction& fun) const {
    for (int x = 0; x < CHUNK_SIZE.x; x++) {
        for (int z = 0; z < CHUNK_SIZE.z; z++) {
            for (int y = 0; y < CHUNK_SIZE.y; y++) {
                fun({ x, y, z }, GetBlock(x, y, z));
            }
        }
    }
}

bool Chunk::IsWithinBounds(int x, int y, int z) const {
    return (x >= 0 && x < CHUNK_SIZE.x) && (y >= 0 && y < CHUNK_SIZE.y) && (z >= 0 && z < CHUNK_SIZE.z);
}

bool Chunk::IsWithinBounds(const glm::ivec3& pos) const {
    return IsWithinBounds(pos.x, pos.y, pos.z);
}

Block Chunk::GetBlock(int x, int y, int z) const {
    return m_blocks[x * CHUNK_SIZE.y * CHUNK_SIZE.z + y * CHUNK_SIZE.z + z];
}

Block Chunk::GetBlock(const glm::ivec3& pos) const {
    return GetBlock(pos.x, pos.y, pos.z);
}
