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

constexpr std::array<glm::ivec3, 6> directions = {{
    {  0,  0, -1 },
    {  0,  0,  1 },
    {  1,  0,  0 },
    { -1,  0,  0 },
    {  0,  1,  0 },
    {  0, -1,  0 }
}};

constexpr auto DirectionToVector(Direction d) -> glm::ivec3 {
    return directions[static_cast<int>(d)];
}

constexpr auto VectorToDirection(const glm::ivec3& v) -> Direction {
    for (int i = 0; i < 6; i++) {
        if (directions[i] == v) {
            return static_cast<Direction>(i);
        }
    }

    assert(false);
    return static_cast<Direction>(-1);
}
