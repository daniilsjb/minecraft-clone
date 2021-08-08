#pragma once

#include "chunk.hpp"
#include "../common/types.hpp"

enum class Biome {
    OCEAN,
    BEACH,
    PLAINS
};

// Generic noise structure that, given a 2D position and a seed, produces a smooth noise value
struct Noise {
    virtual ~Noise() = default;
    virtual auto compute(f32 x, f32 z, f32 seed) -> f32 = 0;
};

// Octave noise which sums up a sequence of values produced by the noise function
// of different frequencies and amplitudes.
struct Octave : public Noise {
    // The number of octaves in the sequence; the amplitude is doubled with each octave,
    // such that the resulting amplitude is equal to 2^(number + 1) - 1
    i32 number;

    // The seed offset for additional control over the output
    i32 offset;

    Octave(i32 t_number, i32 t_offset)
        : number(t_number)
        , offset(t_offset) {}

    auto compute(f32 x, f32 z, f32 seed) -> f32 override;
};

// Combined noise which merges outputs of other noises.
struct Combined : public Noise {
    // The combined noise is defined as compute(x, z) = a.compute(x + b.compute(x, z), z)
    Noise* a;
    Noise* b;

    Combined(Noise* t_a, Noise* t_b)
        : a(t_a)
        , b(t_b) {}

    auto compute(f32 x, f32 z, f32 seed) -> f32 override;
};

void generate(Chunk& chunk, u64 seed);
