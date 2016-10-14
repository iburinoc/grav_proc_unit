#include "gen_rand.hpp"

RealRNG::RealRNG() {}

RealRNG::RealRNG(uint64_t seed) :
	gen(seed) {}

float RealRNG::operator()(float min_val, float max_val) {
	return std::uniform_real_distribution<float>(min_val, max_val)
		(this->gen);
}

float RealRNG::operator()(float max_val) {
	return (*this)(0.0f, max_val);
}

float RealRNG::operator()() {
	return (*this)(0.0f, 1.0f);
}

vec3 rand_sphere(RealRNG &rng) {
	auto dist = std::uniform_real_distribution<float>(-1.0f, 1.0f);
	vec3 res;
	do {
		float x = dist(rng.gen);
		float y = dist(rng.gen);
		float z = dist(rng.gen);

		res = vec3(x, y, z);
	} while(dot(res, res) > 1.0f);

	return res;
}

