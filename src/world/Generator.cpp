#include "Generator.hpp"

#include <PerlinNoise.h>
#include <glm/glm.hpp>

#include "Block.hpp"

constexpr auto Hash(const glm::ivec3& v) -> int64_t {
    int64_t hash = 0;
    hash ^= v.x + 0x9E3779B9ui64 + (hash << 6) + (hash >> 2);
    hash ^= v.y + 0x9E3779B9ui64 + (hash << 6) + (hash >> 2);
    hash ^= v.z + 0x9E3779B9ui64 + (hash << 6) + (hash >> 2);
    return hash;
}

constexpr int water_level = 64;

auto Octave::Compute(float x, float z, float seed) -> float {
    float v = 0.0f;
    float u = 1.0f;
    for (int i = 0; i < number; i++) {
        v += PerlinNoise::Noise(x / u, z / u, seed + i + (offset * 32)) * u;
        u *= 2.0f;
    }
    return v;
}

auto Combined::Compute(float x, float z, float seed) -> float {
    return a->Compute(x + b->Compute(x, z, seed), z, seed);
}

void Generate(Chunk& chunk, const uint64_t seed) {
    srand(seed + Hash(chunk.GetOffset()));

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

    for (int x = 0; x < chunk_size<>.x; x++) {
        for (int z = 0; z < chunk_size<>.z; z++) {
            // Find the block's world position
            int wx = chunk.GetPosition().x + x;
            int wz = chunk.GetPosition().z + z;

            // Sample combined noise functions to retrieve high and low results
            const float scale = 1.3f;
            int hl = (cs[0].Compute(wx * scale, wz * scale, seed) / 6.0f) - 4.0f;
            int hh = (cs[1].Compute(wx * scale, wz * scale, seed) / 5.0f) + 6.0f;

            // Sample the base noise
            float t = n.Compute(wx, wz, seed);

            int hr = 0;
            if (t > 0) {
                hr = hl;
            } else {
                hr = std::max(hl, hh);
            }

            int h = hr + water_level;

            Biome biome;
            if (h < water_level) {
                biome = Biome::OCEAN;
            } else if (t < 0.08f && h < water_level + 2) {
                biome = Biome::BEACH;
            } else {
                biome = Biome::PLAINS;
            }

            for (int y = 0; y < h; y++) {
                unsigned int id;

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

                chunk.SetBlock({ x, y, z }, Block { id });
            }

            for (int y = h; y < water_level; y++) {
                chunk.SetBlock({ x, y, z }, Block { BLOCK_WATER });
            }
        }
    }
}
