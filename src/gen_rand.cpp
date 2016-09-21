#include "gen_rand.hpp"

RealRNG::RealRNG(uint64_t seed, float min_val, float max_val) :
	dist(min_val, max_val), gen(seed) {}

float RealRNG::operator()() {
	return this->dist(this->gen);
}

vec3 rand_sphere(RealRNG &rng) {
	vec3 res;
	do {
		float x = rng();
		float y = rng();
		float z = rng();

		res = vec3(x, y, z);
	} while(dot(res, res) > 1.0f);

	return res;
}

