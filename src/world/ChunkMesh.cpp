#include "ChunkMesh.hpp"
#include "../State.hpp"
#include "../gfx/Renderer.hpp"
#include "../gfx/VertexLayout.hpp"
#include "BlockMesh.hpp"
#include "Chunk.hpp"
#include "World.hpp"

void ChunkMesh::Create() {
    m_vbo.Create(GL_ARRAY_BUFFER);
    m_ibo.Create(GL_ELEMENT_ARRAY_BUFFER);

    VertexLayout layout;
    layout.PushAttribute<float>(3);
    layout.PushAttribute<float>(2);

    m_vao.Create();
    m_vao.Attributes(m_vbo, layout);

    Reset();
}

void ChunkMesh::Destroy() {
    m_vao.Destroy();
    m_ibo.Destroy();
    m_vbo.Destroy();

    Reset();
}

void ChunkMesh::Mesh(const Chunk& target) {
    Reset();
    target.ForEach([this, &target](const glm::ivec3& position, Block block) {
        // No need to mesh air blocks
        if (block.id == BLOCK_AIR) {
            return;
        }

        // Retrieve the type information
        const BlockType& type = Blocks::data[block.id];

        // Create and populate the block mesh structure
        BlockMeshParams mesh {};
        mesh.position = position;

        // Go through each face of the block
        for (int d = 0; d < 6; d++) {
            // Setup the current direction
            mesh.direction = static_cast<Direction>(d);

            // If the face is obscured by another block, skip it
            const glm::ivec3 dir_v = DirectionToVector(mesh.direction);
            const glm::ivec3 neighbor = target.GetPosition() + position + dir_v;

            if (neighbor.y >= 0 && neighbor.y < chunk_size<>.y) {
                const glm::ivec3 offset = BlockToOffset(neighbor);
                const Chunk* chunk = target.GetWorld()->GetChunk(offset);

                if (chunk != nullptr && chunk->GetBlock(BlockToChunk(neighbor)).id != BLOCK_AIR) {
                    continue;
                }
            }

            // Calculate texture coordinates
            const auto [min, max] = State::renderer->atlas.GetCoordinates(type.coords(mesh.direction));
            mesh.uv_min = min;
            mesh.uv_max = max;

            // Append the mesh of the block's face to the current chunk mesh
            mesh.AppendFace(*this);
        }
    });

    // Finalize the mesh
    FinalizeVertices();
    FinalizeIndices();
}

void ChunkMesh::Render() const {
    m_vao.Bind();
    m_ibo.Bind();
    glDrawElements(GL_TRIANGLES, m_index_count, GL_UNSIGNED_INT, nullptr);
}

auto ChunkMesh::GetVertexCount() const -> unsigned int {
    return m_vertex_count;
}

auto ChunkMesh::GetIndexCount() const -> unsigned int {
    return m_index_count;
}

void ChunkMesh::Reset() {
    m_vertex_count = 0;
    m_index_count = 0;
}

void ChunkMesh::FinalizeVertices() {
    m_vbo.Buffer(m_vertices.data(), sizeof(ChunkVertex) * m_vertices.size());
    m_vertices.clear();
}

void ChunkMesh::FinalizeIndices() {
    m_ibo.Buffer(m_indices.data(), sizeof(ChunkIndex) * m_indices.size());
    m_indices.clear();
}
