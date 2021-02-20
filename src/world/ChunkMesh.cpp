#include "ChunkMesh.hpp"
#include "../State.hpp"
#include "../gfx/Renderer.hpp"
#include "../gfx/VertexLayout.hpp"
#include "BlockMesh.hpp"
#include "Chunk.hpp"
#include "World.hpp"

#include <algorithm>

void ChunkMesh::Create() {
    m_vbo.Create(GL_ARRAY_BUFFER);
    m_ibo.Create(GL_ELEMENT_ARRAY_BUFFER);

    VertexLayout layout;
    layout.PushAttribute<float>(3);
    layout.PushAttribute<float>(2);
    layout.PushAttribute<float>(3);

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

void ChunkMesh::Mesh(const Chunk& target, bool transparent) {
    Reset();
    target.ForEach([this, &target, transparent](const glm::ivec3& position, Block block) {
        // No need to mesh air blocks
        if (block.id == BLOCK_AIR) {
            return;
        }

        // Retrieve the type information
        const BlockType& type = Blocks::data[block.id];

        // Ensure we're only meshing blocks of requested transparency
        if (type.transparent != transparent) {
            return;
        }

        // Create and populate the block mesh structure
        BlockMeshParams mesh {};
        mesh.position = position;
        mesh.chunk_position = target.GetPosition();
        mesh.transparent = type.transparent;
        mesh.liquid = type.liquid;

        if (type.sprite) {
            const auto [min, max] = State::renderer->atlas.GetCoordinates(type.coords(NORTH));
            mesh.uv_min = min;
            mesh.uv_max = max;

            mesh.AppendSprite(*this);
            return;
        }

        // Go through each face of the block
        for (int d = 0; d < 6; d++) {
            // Setup the current direction
            mesh.direction = static_cast<Direction>(d);

            // If the face is obscured by another block, skip it
            const glm::ivec3 dir_v = DirectionToVector(mesh.direction);
            const glm::ivec3 neighbor_pos = target.GetPosition() + position + dir_v;

            Block neighbor = target.GetWorld()->GetBlock(neighbor_pos);
            const BlockType& neighbor_type = Blocks::data[neighbor.id];

            // Skip hidden block faces
            if (neighbor_type.transparent && (!type.transparent || type.id != neighbor_type.id)) {
                // Calculate texture coordinates
                const auto [min, max] = State::renderer->atlas.GetCoordinates(type.coords(mesh.direction));
                mesh.uv_min = min;
                mesh.uv_max = max;

                // Append the mesh of the block's face to the current chunk mesh
                mesh.AppendFace(*this);
            }
        }
    });

    // Finalize the mesh
    SortFaces();
    FinalizeVertices();
    FinalizeIndices();
}

void ChunkMesh::Sort() {
    SortFaces();
    FinalizeIndices();
}

void ChunkMesh::Render() const {
    if (m_index_count == 0) {
        return;
    }

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

    m_faces.clear();
    m_indices.clear();
}

void ChunkMesh::SortFaces() {
    if (m_faces.empty()) {
        return;
    }

    for (auto& face : m_faces) {
        face.distance = glm::distance(State::renderer->camera.position, face.position);
    }

    std::sort(m_faces.begin(), m_faces.end(), [](const ChunkFace& a, const ChunkFace& b) {
        return a.distance > b.distance;
    });

    std::vector<ChunkIndex> sorted_indices;
    for (size_t i = 0; i < m_faces.size(); i++) {
        ChunkFace& face = m_faces[i];
        sorted_indices.insert(sorted_indices.end(), std::make_move_iterator(m_indices.begin() + face.index_start), std::make_move_iterator(m_indices.begin() + face.index_start + 6));
        face.index_start = i * 6;
    }

    m_indices.swap(sorted_indices);
}

void ChunkMesh::FinalizeVertices() {
    m_vbo.Buffer(m_vertices.data(), sizeof(ChunkVertex) * m_vertices.size());
    m_vertices.clear();
}

void ChunkMesh::FinalizeIndices() {
    m_ibo.Buffer(m_indices.data(), sizeof(ChunkIndex) * m_indices.size());
    // m_indices.clear();
}
