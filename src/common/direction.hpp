#pragma once

#include <glm/glm.hpp>

#include <array>
#include <cassert>

enum Direction {
    NORTH = 0,
    SOUTH = 1,
    EAST = 2,
    WEST = 3,
    UP = 4,
    DOWN = 5
};

constexpr std::array<glm::ivec3, 6> DIRECTIONS = {{
    {  0,  0, -1 },
    {  0,  0,  1 },
    {  1,  0,  0 },
    { -1,  0,  0 },
    {  0,  1,  0 },
    {  0, -1,  0 }
}};

constexpr auto direction_to_vector(Direction d) -> glm::ivec3 {
    return DIRECTIONS[static_cast<size_t>(d)];
}

constexpr auto vector_to_direction(const glm::ivec3& v) -> Direction {
    for (size_t i = 0; i < 6; i++) {
        if (DIRECTIONS[i] == v) {
            return static_cast<Direction>(i);
        }
    }

    // TODO: Fix this
    assert(false);
    return static_cast<Direction>(0);
}
