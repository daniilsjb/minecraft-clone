#pragma once

#include <glm/glm.hpp>

#include <array>
#include <functional>

#include "../common/direction.hpp"
#include "../common/types.hpp"

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
    BLOCK_RED_FLOWER,
    BLOCK_YELLOW_FLOWER,
    BLOCK_WOOD,
    BLOCK_COUNT
};

// Static information about each block type
struct BlockType {
    BlockId id;
    bool transparent;
    bool liquid;
    bool sprite;
    std::function<glm::ivec2(Direction)> coords;
};

// Static id-based "table" of block types
struct Blocks {
    static inline const std::array<BlockType, BLOCK_COUNT> data = {{
        { BLOCK_AIR, true, false, false, [](Direction d) -> glm::ivec2 {
             return { -1, -1 };
        }},

        { BLOCK_COARSE_DIRT, false, false, false, [](Direction d) -> glm::ivec2 {
             return { 0, 0 };
        }},

        { BLOCK_DIRT, false, false, false, [](Direction d) -> glm::ivec2 {
             return { 1, 0 };
        }},

        { BLOCK_GRASS, false, false, false, [](Direction d) -> glm::ivec2 {
             switch (d) {
                 case UP: return { 3, 0 };
                 case DOWN: return { 1, 0 };
                 default: return { 2, 0 };
             }
        }},

        { BLOCK_BEDROCK, false, false, false, [](Direction d) -> glm::ivec2 {
             return { 4, 0 };
        }},

        { BLOCK_STONE, false, false, false, [](Direction d) -> glm::ivec2 {
             return { 5, 0 };
        }},

        { BLOCK_LEAVES, true, false, false, [](Direction d) -> glm::ivec2 {
             return { 6, 0 };
        }},

        { BLOCK_OAK, false, false, false, [](Direction d) -> glm::ivec2 {
             switch (d) {
                 case UP:
                 case DOWN: return { 8, 0 };
                 default: return { 7, 0 };
             }
        }},

        { BLOCK_SAND, false, false, false, [](Direction d) -> glm::ivec2 {
             return { 9, 0 };
        }},

        { BLOCK_WATER, true, true, false, [](Direction d) -> glm::ivec2 {
             return { 10, 0 };
        }},

        { BLOCK_GLASS, true, false, false, [](Direction d) -> glm::ivec2 {
             return { 11, 0 };
        }},

        { BLOCK_RED_FLOWER, true, false, true, [](Direction d) -> glm::ivec2 {
             return { 12, 0 };
        }},

        { BLOCK_YELLOW_FLOWER, true, false, true, [](Direction d) -> glm::ivec2 {
             return { 13, 0 };
        }},

        { BLOCK_WOOD, false, false, false, [](Direction d) -> glm::ivec2 {
             return { 14, 0 };
        }},
    }};
};

// Block instances as stored in chunks
struct Block {
    u32 id;
};
