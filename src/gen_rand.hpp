#pragma once

#include <random>

#include <stdint.h>

#include "gpu/glm.hpp"

class RealRNG {
	std::mt19937_64 gen;

public:
	RealRNG();
	RealRNG(uint64_t seed);
	float operator()(float min_val, float max_val);
	float operator()(float max_val);
	float operator()();

	friend vec3 rand_sphere(RealRNG &rng);
};

//vec3 rand_sphere(RealRNG &rng);

