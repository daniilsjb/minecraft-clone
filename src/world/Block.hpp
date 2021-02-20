#pragma once

#include <glm/glm.hpp>

#include <array>
#include <functional>

#include "../general/Direction.hpp"

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
    BLOCK_SAND,
    BLOCK_WATER,
    BLOCK_GLASS,
    BLOCK_COUNT
};

// Static information about each block type
struct BlockType {
    BlockId id;
    bool transparent;
    bool liquid;
    std::function<glm::ivec2(Direction)> coords;
};

// Static id-based "table" of block types
struct Blocks {
    static inline const std::array<BlockType, BLOCK_COUNT> data = {{
        { BLOCK_AIR, true, false, [](Direction d) -> glm::ivec2 {
             return { -1, -1 };
        }},

        { BLOCK_COARSE_DIRT, false, false, [](Direction d) -> glm::ivec2 {
             return { 0, 0 };
        }},

        { BLOCK_DIRT, false, false, [](Direction d) -> glm::ivec2 {
             return { 1, 0 };
        }},

        { BLOCK_GRASS, false, false, [](Direction d) -> glm::ivec2 {
             switch (d) {
                 case UP: return { 3, 0 };
                 case DOWN: return { 1, 0 };
                 default: return { 2, 0 };
             }
        }},

        { BLOCK_BEDROCK, false, false, [](Direction d) -> glm::ivec2 {
             return { 4, 0 };
        }},

        { BLOCK_STONE, false, false, [](Direction d) -> glm::ivec2 {
             return { 5, 0 };
        }},

        { BLOCK_LEAVES, true, false, [](Direction d) -> glm::ivec2 {
             return { 6, 0 };
        }},

        { BLOCK_OAK, false, false, [](Direction d) -> glm::ivec2 {
             switch (d) {
                 case UP:
                 case DOWN: return { 8, 0 };
                 default: return { 7, 0 };
             }
        }},

        { BLOCK_SAND, false, false, [](Direction d) -> glm::ivec2 {
             return { 9, 0 };
        }},

        { BLOCK_WATER, true, true, [](Direction d) -> glm::ivec2 {
             return { 10, 0 };
        }},

        { BLOCK_GLASS, true, false, [](Direction d) -> glm::ivec2 {
             return { 11, 0 };
        }},
    }};
};

// Block instance as stored in chunks
struct Block {
    unsigned int id;
};
