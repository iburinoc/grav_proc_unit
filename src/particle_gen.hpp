#pragma once

#include <tuple>
#include <stdint.h>

#include "gpu/glm.hpp"

#include "gen_rand.hpp"

class ParticleGen {
	uint64_t seed;
	RealRNG rng;

public:
	ParticleGen(uint64_t seed);

	void reset();

	std::tuple<vec3, vec3, float, float> next_particle();
};

