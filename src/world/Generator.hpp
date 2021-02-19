#pragma once

#include "Chunk.hpp"

#include <cstdint>

enum class Biome {
    OCEAN,
    BEACH,
    PLAINS
};

// Generic noise structure that, given a 2D position and a seed, produces a smooth noise value
struct Noise {
    virtual auto Compute(float x, float z, float seed) -> float = 0;
};

// Octave noise which sums up a sequence of values produced by the noise function
// of different frequencies and amplitudes.
struct Octave : public Noise {
    // The number of octaves in the sequence; the amplitude is doubled with each octave,
    // such that the resulting amplitude is equal to 2^(number + 1) - 1
    int number;

    // The seed offset for additional control over the output
    int offset;

    Octave(int number, int offset)
        : number(number)
        , offset(offset) {
    }

    auto Compute(float x, float z, float seed) -> float override;
};

// Combined noise which merges outputs of other noises.
struct Combined : public Noise {
    // The combined noise is defined as compute(x, z) = a.compute(x + b.compute(x, z), z)
    Noise* a;
    Noise* b;

    Combined(Noise* a, Noise* b)
        : a(a)
        , b(b) {
    }

    auto Compute(float x, float z, float seed) -> float override;
};

void Generate(Chunk& chunk, const uint64_t seed);
