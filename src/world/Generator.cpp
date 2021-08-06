#include "Generator.hpp"

#include <db_perlin.hpp>

#include "../world/World.hpp"

static auto random(const int min, const int max) -> int {
    return (rand() % (max - min + 1)) + min;
}

static auto chance(const double percent) -> bool {
    return static_cast<double>(random(0, 100000)) / 100000.0 <= percent;
}

constexpr auto hash(const glm::ivec3& v) -> int64_t {
    uint64_t hash = 0;
    hash ^= static_cast<uint64_t>(v.x) + 0x9E3779B9ULL + (hash << 6) + (hash >> 2);
    hash ^= static_cast<uint64_t>(v.y) + 0x9E3779B9ULL + (hash << 6) + (hash >> 2);
    hash ^= static_cast<uint64_t>(v.z) + 0x9E3779B9ULL + (hash << 6) + (hash >> 2);
    return static_cast<int64_t>(hash);
}

constexpr int WATER_LEVEL = 64;

auto Octave::compute(float x, float z, float seed) -> float {
    float v = 0.0f;
    float u = 1.0f;
    for (int i = 0; i < number; i++) {
        v += db::perlin(x / u, z / u, seed + float(i) + (float(offset) * 32.0f)) * u;
        u *= 2.0f;
    }
    return v;
}

auto Combined::compute(float x, float z, float seed) -> float {
    return a->compute(x + b->compute(x, z, seed), z, seed);
}

static void put_tree(Chunk& chunk, int x, int y, int z) {
    int height = random(4, 6);

    Block trunk = { BLOCK_OAK };
    Block leaves = { BLOCK_LEAVES };

    for (int i = 0; i < height; i++) {
        chunk.set_block({ x, y + i, z }, trunk);
    }

    int top_start = y + height - 1;
    int top = top_start;

    int radius = random(2, 3);
    while (radius > 0) {
        for (int dx = -radius; dx <= radius; dx++) {
            for (int dz = -radius; dz <= radius; dz++) {
                if (top == top_start && dx == 0 && dz == 0) {
                    continue;
                }

                bool corner = (std::abs(dx) == radius) && (std::abs(dz) == radius);
                if (corner && chance(0.15)) {
                    continue;
                }

                chunk.get_world()->set_block(chunk.get_position() + glm::ivec3 { x + dx, top, z + dz }, leaves);
            }
        }

        top++;
        radius--;
    }
}

static void put_flower(Chunk& chunk, int x, int y, int z) {
    unsigned int id = BLOCK_RED_FLOWER;
    switch (random(0, 1)) {
        case 0: id = BLOCK_RED_FLOWER; break; 
        case 1: id = BLOCK_YELLOW_FLOWER; break; 
    }

    chunk.set_block({ x, y, z }, Block { id });
}

void generate(Chunk& chunk, const uint64_t seed) {
    srand(static_cast<unsigned int>(seed + static_cast<uint64_t>(hash(chunk.get_offset()))));

    // Base noise
    Octave n = { 6, 0 };

    // Several pre-defined octaves with different offsets
    Octave os[] = {
        { 8, 1 },
        { 8, 2 },
        { 8, 3 },
        { 8, 4 },
    };

    // Several pre-defined combined noises merged from different octave
    // noises
    Combined cs[] = {
        { &os[0], &os[1] },
        { &os[2], &os[3] },
    };

    for (int x = 0; x < CHUNK_SIZE<>.x; x++) {
        for (int z = 0; z < CHUNK_SIZE<>.z; z++) {
            // Find the block's world position
            float wx = static_cast<float>(chunk.get_position().x + x);
            float wz = static_cast<float>(chunk.get_position().z + z);

            // Sample combined noise functions to retrieve high and low results
            const float scale = 1.3f;
            float hl = (cs[0].compute(wx * scale, wz * scale, static_cast<float>(seed)) / 6.0f) - 4.0f;
            float hh = (cs[1].compute(wx * scale, wz * scale, static_cast<float>(seed)) / 5.0f) + 6.0f;

            // Sample the base noise
            float t = n.compute(wx, wz, static_cast<float>(seed));

            float hr;
            if (t > 0) {
                hr = hl;
            } else {
                hr = std::max(hl, hh);
            }

            int h = static_cast<int>(hr) + WATER_LEVEL;

            Biome biome;
            if (h < WATER_LEVEL) {
                biome = Biome::OCEAN;
            } else if (t < 0.08f && h < WATER_LEVEL + 2) {
                biome = Biome::BEACH;
            } else {
                biome = Biome::PLAINS;
            }

            for (int y = 0; y < h; y++) {
                unsigned int id = 0;

                // Determine the top-block based on the biome
                if (y == (h - 1)) {
                    switch (biome) {
                        case Biome::OCEAN: {
                            id = BLOCK_DIRT;
                            break;
                        }
                        case Biome::BEACH: {
                            id = (t > 0.03f) ? BLOCK_DIRT : BLOCK_SAND;
                            break;
                        }
                        case Biome::PLAINS: {
                            id = BLOCK_GRASS;
                            break;
                        }
                    }
                } else if (y > (h - 4)) {
                    id = (biome == Biome::BEACH) ? BLOCK_SAND : BLOCK_DIRT;
                } else {
                    id = BLOCK_STONE;
                }

                chunk.set_block({ x, y, z }, Block { id });
            }

            for (int y = h; y < WATER_LEVEL; y++) {
                chunk.set_block({ x, y, z }, Block { BLOCK_WATER });
            }

            if (biome == Biome::PLAINS) {
                if (chance(0.002)) {
                    put_tree(chunk, x, h, z);
                } else if (chance(0.01)) {
                    put_flower(chunk, x, h, z);
                }
            }
        }
    }
}
