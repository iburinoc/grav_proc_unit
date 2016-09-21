#include "../backend.hpp"
#include "../gen_rand.hpp"
#include "../options.hpp"

BasicBackend::BasicBackend(int count) : Backend(count),
		pos(this->count),
		vel(this->count),
		acc(this->count) {
	RealRNG rng(options.seed, -1.0f, 1.0f);

	for(int i = 0; i < this->count; i++) {
		this->pos[i] = vec4(rand_sphere(rng), 0.005f);
		this->vel[i] = rand_sphere(rng);
	}
}

BasicBackend::~BasicBackend() {}

void BasicBackend::update(double dt) {
	this->sb->set_vertices(this->pos);
}

