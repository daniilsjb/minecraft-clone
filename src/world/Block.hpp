#pragma once

#include <glm/glm.hpp>

#include <array>
#include <functional>

#include "Direction.hpp"

// Unique identifier for each block type
enum BlockId {
    BLOCK_AIR,
    BLOCK_COARSE_DIRT,
    BLOCK_DIRT,
    BLOCK_GRASS,
    BLOCK_BEDROCK,
    BLOCK_STONE,
    BLOCK_LEAVES,
    BLOCK_OAK,
    BLOCK_COUNT
};

// Static information about each block type
struct BlockType {
    BlockId id;

    std::function<glm::ivec2(Direction)> coords;
};

// Static id-based "table" of block types
struct Blocks {
    static inline const std::array<BlockType, BLOCK_COUNT> data = {{
        { BLOCK_AIR, [](Direction d) -> glm::ivec2 {
             return { -1, -1 };
        }},

        { BLOCK_COARSE_DIRT, [](Direction d) -> glm::ivec2 {
             return { 0, 0 };
        }},

        { BLOCK_DIRT, [](Direction d) -> glm::ivec2 {
             return { 1, 0 };
        }},

        { BLOCK_GRASS, [](Direction d) -> glm::ivec2 {
             switch (d) {
                 case UP: return { 3, 0 };
                 case DOWN: return { 1, 0 };
                 default: return { 2, 0 };
             }
        }},

        { BLOCK_BEDROCK, [](Direction d) -> glm::ivec2 {
             return { 4, 0 };
        }},

        { BLOCK_STONE, [](Direction d) -> glm::ivec2 {
             return { 5, 0 };
        }},

        { BLOCK_LEAVES, [](Direction d) -> glm::ivec2 {
             return { 6, 0 };
        }},

        { BLOCK_OAK, [](Direction d) -> glm::ivec2 {
             switch (d) {
                 case UP:
                 case DOWN: return { 8, 0 };
                 default: return { 7, 0 };
             }
        }},
    }};
};

// Block instance as stored in chunks
struct Block {
    BlockId id;
};
