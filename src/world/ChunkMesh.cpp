#include "ChunkMesh.hpp"

#include <iostream>

#include "../State.hpp"
#include "../gfx/Renderer.hpp"
#include "../gfx/VertexLayout.hpp"
#include "BlockMesh.hpp"
#include "Chunk.hpp"
#include "World.hpp"

ChunkMesh::ChunkMesh(Chunk* chunk) :
    chunk(chunk),
    vbo(GL_ARRAY_BUFFER),
    ibo(GL_ELEMENT_ARRAY_BUFFER) {
    VertexLayout layout;
    layout.PushAttribute<float>(3);
    layout.PushAttribute<float>(2);

    vao.SetAttributes(vbo, layout);
}

void ChunkMesh::Mesh() {
    vertexCount = 0;
    indexCount = 0;

    chunk->ForEach([this](const glm::ivec3& pos, Block block) {
        // No need to mesh air blocks
        if (block.id == BLOCK_AIR) {
            return;
        }

        // Retrieve the type information
        const BlockType& type = Blocks::data[block.id];

        // Create and populate the block mesh structure
        BlockMesh mesh {};
        mesh.pos = pos;

        // Go through each face of the block
        for (int d = 0; d < 6; d++) {
            // Setup the current direction
            mesh.dir = static_cast<Direction>(d);

            // If the face is obscured by another block, skip it
            glm::ivec3 neighbor = pos + DirectionToVector(mesh.dir);
            if (chunk->world->IsSolidBlock((chunk->location * CHUNK_SIZE) + neighbor)) {
                continue;
            }

            // Calculate texture coordinates
            auto [min, max] = State::renderer->atlas.GetCoordinates(type.coords(mesh.dir));
            mesh.uvMin = min;
            mesh.uvMax = max;

            // Append the mesh of the block's face to the current chunk mesh
            mesh.AppendFace(this);
        }
    });

    // Finalize the mesh
    FinalizeVertices();
    FinalizeIndices();
}

void ChunkMesh::Render() const {
    vao.Bind();
    ibo.Bind();

    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
}

void ChunkMesh::FinalizeVertices() {
    vbo.Generate(vertices.data(), sizeof(ChunkVertex) * vertices.size());
    vertices.clear();
}

void ChunkMesh::FinalizeIndices() {
    ibo.Generate(indices.data(), sizeof(ChunkIndex) * indices.size());
    indices.clear();
}
