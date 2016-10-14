#include "particle_gen.hpp"

ParticleGen::ParticleGen(uint64_t seed) : seed(seed) {
	this->reset();
}

void ParticleGen::reset() {
	this->rng = RealRNG(seed);
}

std::tuple<vec3, vec3, float, float> ParticleGen::next_particle() {
	vec3 pos = rand_sphere(rng);
	vec3 vel = glm::normalize(glm::cross(pos, rand_sphere(rng))) * .25f;

	float size = 0.01f;
	float m = size * size * size;

	return std::make_tuple(pos, vel, size, m);
}

