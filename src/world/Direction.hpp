#pragma once

#include <glm/glm.hpp>

enum Direction {
    NORTH = 0,
    SOUTH = 1,
    EAST = 2,
    WEST = 3,
    UP = 4,
    DOWN = 5
};

static inline glm::ivec3 DirectionToVector(Direction d) {
    switch (d) {
        case NORTH: return {  0,  0, -1 };
        case SOUTH: return {  0,  0,  1 };
        case EAST:  return {  1,  0,  0 };
        case WEST:  return { -1,  0,  0 };
        case UP:    return {  0,  1,  0 };
        case DOWN:  return {  0, -1,  0 };
        default:    return {  0,  0,  0 };
    }
}
