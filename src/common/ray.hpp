#pragma once

#include <glm/glm.hpp>

#include <optional>
#include <functional>

#include "direction.hpp"
#include "types.hpp"

struct RayHit {
    // Position of the block that the ray hit
    glm::ivec3 position;

    // Direction of the block's face that the ray hit
    Direction face;
};

// Given a position, tells whether it contains a solid block
using RayIntersect = std::function<bool(const glm::ivec3& position)>;

struct Ray {
    glm::vec3 origin;
    glm::vec3 direction;

    [[nodiscard]]
    auto cast(f32 max_distance, const RayIntersect& intersect) const -> std::optional<RayHit>;
};
