#include "chunk_mesh.hpp"
#include "../state.hpp"
#include "../gfx/renderer.hpp"
#include "block_mesh.hpp"
#include "chunk.hpp"
#include "world.hpp"

void ChunkMesh::create() {
    m_vbo.create(GL_ARRAY_BUFFER);
    m_ibo.create(GL_ELEMENT_ARRAY_BUFFER);

    VertexLayout layout;
    layout.push_attribute<f32>(3);
    layout.push_attribute<f32>(2);
    layout.push_attribute<f32>(3);

    m_vao.create();
    m_vao.attributes(m_vbo, layout);

    reset();
}

void ChunkMesh::destroy() {
    m_vao.destroy();
    m_ibo.destroy();
    m_vbo.destroy();

    reset();
}

void ChunkMesh::mesh(const Chunk& target, bool transparent) {
    reset();
    target.for_each([this, &target, transparent](const glm::ivec3& position, Block block) {
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
        mesh.chunk_position = target.get_position();
        mesh.transparent = type.transparent;
        mesh.liquid = type.liquid;

        if (type.sprite) {
            const auto [min, max] = State::renderer->atlas.get_coordinates(type.coords(NORTH));
            mesh.uv_min = min;
            mesh.uv_max = max;

            mesh.append_sprite(*this);
            return;
        }

        // Go through each face of the block
        for (int d = 0; d < 6; d++) {
            // Set up the current direction
            mesh.direction = static_cast<Direction>(d);

            // If the face is obscured by another block, skip it
            const glm::ivec3 dir_v = direction_to_vector(mesh.direction);
            const glm::ivec3 neighbor_pos = target.get_position() + position + dir_v;

            Block neighbor = target.get_world()->get_block(neighbor_pos);
            const BlockType& neighbor_type = Blocks::data[neighbor.id];

            // Skip hidden block faces
            if (neighbor_type.transparent && (!type.transparent || type.id != neighbor_type.id)) {
                // Calculate texture coordinates
                const auto [min, max] = State::renderer->atlas.get_coordinates(type.coords(mesh.direction));
                mesh.uv_min = min;
                mesh.uv_max = max;

                // Append the mesh of the block's face to the current chunk mesh
                mesh.append_face(*this);
            }
        }
    });

    // Finalize the mesh
    sort_faces();
    finalize_vertices();
    finalize_indices();
}

void ChunkMesh::sort() {
    sort_faces();
    finalize_indices();
}

void ChunkMesh::render() const {
    if (m_index_count == 0) {
        return;
    }

    m_vao.bind();
    m_ibo.bind();
    glDrawElements(GL_TRIANGLES, static_cast<i32>(m_index_count), GL_UNSIGNED_INT, nullptr);
}

auto ChunkMesh::get_vertex_count() const -> u32 {
    return m_vertex_count;
}

auto ChunkMesh::get_index_count() const -> u32 {
    return m_index_count;
}

void ChunkMesh::reset() {
    m_vertex_count = 0;
    m_index_count = 0;

    m_faces.clear();
    m_indices.clear();
}

void ChunkMesh::sort_faces() {
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
        sorted_indices.insert(
            sorted_indices.end(),
            std::make_move_iterator(m_indices.begin() + static_cast<i32>(face.index_start)),
            std::make_move_iterator(m_indices.begin() + static_cast<i32>(face.index_start) + 6)
        );
        face.index_start = i * 6;
    }

    m_indices.swap(sorted_indices);
}

void ChunkMesh::finalize_vertices() {
    m_vbo.buffer(m_vertices.data(), static_cast<u32>(sizeof(ChunkVertex) * m_vertices.size()));
    m_vertices.clear();
}

void ChunkMesh::finalize_indices() {
    m_ibo.buffer(m_indices.data(), static_cast<u32>(sizeof(ChunkIndex) * m_indices.size()));
    // m_indices.clear();
}
