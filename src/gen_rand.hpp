#pragma once

#include <random>

#include <stdint.h>

#include "gpu/glm.hpp"

class RealRNG {
	std::uniform_real_distribution<float> dist;
	std::mt19937_64 gen;

public:
	RealRNG(uint64_t seed, float min_val=0.0f, float max_val=1.0f);
	float operator()();
};

vec3 rand_sphere(RealRNG &rng);

