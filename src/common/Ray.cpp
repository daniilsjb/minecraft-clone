#include "Ray.hpp"

auto Ray::Cast(float max_distance, const RayIntersect& intersect) const -> std::optional<RayHit> {
    glm::ivec3 position = glm::floor(origin);
    glm::ivec3 step = glm::sign(direction);

    glm::vec3 tmax = {
        ((direction.x > 0) ? (ceilf(origin.x) - origin.x) : (origin.x - floorf(origin.x))) / fabsf(direction.x),
        ((direction.y > 0) ? (ceilf(origin.y) - origin.y) : (origin.y - floorf(origin.y))) / fabsf(direction.y),
        ((direction.z > 0) ? (ceilf(origin.z) - origin.z) : (origin.z - floorf(origin.z))) / fabsf(direction.z)
    };
    glm::vec3 tdelta = (glm::vec3)step / direction;

    float radius = max_distance / glm::length(direction);

    Direction face = (Direction)-1;
    while (true) {
        if (intersect(position)) {
            return RayHit { position, face };
        }

        if (tmax.x < tmax.y) {
            if (tmax.x < tmax.z) {
                if (tmax.x > radius) {
                    break;
                }

                position.x += step.x;
                tmax.x += tdelta.x;
                face = VectorToDirection({ -step.x, 0, 0 });
            } else {
                if (tmax.z > radius) {
                    break;
                }

                position.z += step.z;
                tmax.z += tdelta.z;
                face = VectorToDirection({ 0, 0, -step.z });
            }
        } else {
            if (tmax.y < tmax.z) {
                if (tmax.y > radius) {
                    break;
                }

                position.y += step.y;
                tmax.y += tdelta.y;
                face = VectorToDirection({ 0, -step.y, 0 });
            } else {
                if (tmax.z > radius) {
                    break;
                }

                position.z += step.z;
                tmax.z += tdelta.z;
                face = VectorToDirection({ 0, 0, -step.z });
            }
        }
    }

    return {};
}