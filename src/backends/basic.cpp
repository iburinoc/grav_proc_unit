#include "../backend.hpp"
#include "../gen_rand.hpp"
#include "../options.hpp"
#include "../consts.hpp"

BasicBackend::BasicBackend(int count) : Backend(count),
		pos(this->count),
		vel(this->count),
		x2(this->count),
		x3(this->count),
		x4(this->count),
		v2(this->count),
		v3(this->count),
		v4(this->count),
		aa(this->count) {
	RealRNG rng(options.seed, -1.0f, 1.0f);

	this->pos[0] = vec4(0, 0, 0, 0.5f);
	this->vel[0] = vec3(0, 0, -.05f);

	this->pos[1] = vec4(0, 1, 0, 0.5f);
	this->vel[1] = vec3(0, 0, 0.05f);

	for(int i = 2; i < this->count; i++) {
		this->pos[i] = vec4(rand_sphere(rng), 0.005f);
		this->vel[i] = glm::normalize(glm::cross(vec3(this->pos[i]), rand_sphere(rng))) * 0.25f;
	}
}

BasicBackend::~BasicBackend() {}

static vec3 grav_force(vec4 o1, vec4 o2) {
	vec3 delta = vec3(o1) - vec3(o2);
	float l = length(delta);
	float d = l*l + Geps * Geps;
	return (float)-G * o2.w / d / l * delta;
}

void BasicBackend::update(float dt) {
	for(int i = 0; i < this->count; i++) {
		vec3 acc = vec3(0, 0, 0);
		for(int j = 0; j < this->count; j++) {
			if(i == j) continue;
			acc += grav_force(this->pos[i], this->pos[j]);
		}
		this->x2[i] = this->pos[i] + vec4(.5f * dt * this->vel[i], .0f);
		this->v2[i] = this->vel[i] + .5f * dt * acc;

		this->aa[i] += acc;
	}
	for(int i = 0; i < this->count; i++) {
		vec3 acc = vec3(0, 0, 0);
		for(int j = 0; j < this->count; j++) {
			if(i == j) continue;
			acc += grav_force(this->x2[i], this->x2[j]);
		}
		this->x3[i] = this->pos[i] + vec4(.5f * dt * this->v2[i], .0f);
		this->v3[i] = this->vel[i] + .5f * dt * acc;

		this->aa[i] += 2.0f*acc;
	}
	for(int i = 0; i < this->count; i++) {
		vec3 acc = vec3(0, 0, 0);
		for(int j = 0; j < this->count; j++) {
			if(i == j) continue;
			acc += grav_force(this->x3[i], this->x3[j]);
		}
		this->x4[i] = this->pos[i] + vec4(dt * this->v3[i], .0f);
		this->v4[i] = this->vel[i] + dt * acc;

		this->aa[i] += 2.0f*acc;
	}
	for(int i = 0; i < this->count; i++) {
		vec3 acc = vec3(0, 0, 0);
		for(int j = 0; j < this->count; j++) {
			if(i == j) continue;
			acc += grav_force(this->x4[i], this->x4[j]);
		}

		this->pos[i] += vec4(dt / 6.f *
			(this->vel[i] +
			 2.f * this->v2[i] +
			 2.f * this->v3[i] +
			 this->v4[i]), 0.f);

		this->vel[i] += dt / 6.f * (this->aa[i] + acc); 
	}
	this->sb->set_vertices(this->pos);
}

