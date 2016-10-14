#include <iostream>

#include "gpu/gl.hpp"

#include "../backend.hpp"
#include "../consts.hpp"
#include "../gen_rand.hpp"
#include "../gl_util.hpp"
#include "../log.hpp"
#include "../options.hpp"

#include "basic.hpp"

BasicBackend::BasicBackend(int count, std::unique_ptr<ParticleGen> p_gen) :
		Backend(count, std::move(p_gen), sizeof(vec3)),
		pos(this->count),
		vel(this->count),
		size(this->count),
		m(this->count),
		active(this->count),
		x2(this->count),
		x3(this->count),
		x4(this->count),
		v2(this->count),
		v3(this->count),
		v4(this->count),
		aa(this->count) {
	RealRNG rng(options.seed);

	//this->pos[0] = vec4(0, 0, 0, 0.5f);
	//this->vel[0] = vec3(0, 0, -.25f);

	//this->pos[1] = vec4(0, 1, 0, 0.5f);
	//this->vel[1] = vec3(0, 0, 0.25f);

	for(int i = 0; i < this->count; i++) {
		this->pos[i] = rand_sphere(rng);
		this->size[i] = 0.05f;
		this->vel[i] = glm::normalize(glm::cross(this->pos[i], rand_sphere(rng))) * 0.25f;
		this->m[i] = 0.25f*0.25f*0.25f;
		this->active[i] = true;
	}
}

BasicBackend::~BasicBackend() {}

static vec3 grav_accel(vec3 delta, float mass) {
	float l = length(delta);
	float d = l*l + Geps * Geps;
	return (float)-G * mass / d / l * delta;
}

void BasicBackend::update(float dt) {
	for(int i = 0; i < this->count; i++) {
		if(!this->active[i]) continue;
		vec3 acc = vec3(0, 0, 0);
		for(int j = 0; j < this->count; j++) {
			if(!this->active[j]) continue;
			if(i == j) continue;
			acc += grav_accel(this->pos[i]-this->pos[j],
				this->size[j]);
		}
		this->x2[i] = this->pos[i] + .5f * dt * this->vel[i];
		this->v2[i] = this->vel[i] + .5f * dt * acc;

		this->aa[i] = acc;
	}
	for(int i = 0; i < this->count; i++) {
		if(!this->active[i]) continue;
		vec3 acc = vec3(0, 0, 0);
		for(int j = 0; j < this->count; j++) {
			if(!this->active[j]) continue;
			if(i == j) continue;
			acc += grav_accel(this->x2[i]-this->x2[j],
				this->size[j]);
		}
		this->x3[i] = this->pos[i] + .5f * dt * this->v2[i];
		this->v3[i] = this->vel[i] + .5f * dt * acc;

		this->aa[i] += 2.0f*acc;
	}
	for(int i = 0; i < this->count; i++) {
		if(!this->active[i]) continue;
		vec3 acc = vec3(0, 0, 0);
		for(int j = 0; j < this->count; j++) {
			if(!this->active[j]) continue;
			if(i == j) continue;
			acc += grav_accel(this->x3[i]-this->x3[j],
				this->size[j]);
		}
		this->x4[i] = this->pos[i] + dt * this->v3[i];
		this->v4[i] = this->vel[i] + dt * acc;

		this->aa[i] += 2.0f*acc;
	}
	for(int i = 0; i < this->count; i++) {
		if(!this->active[i]) continue;
		vec3 acc = vec3(0, 0, 0);
		for(int j = 0; j < this->count; j++) {
			if(!this->active[j]) continue;
			if(i == j) continue;
			acc += grav_accel(this->x4[i]-this->x4[j],
				this->size[j]);
		}

		this->pos[i] += dt / 6.f *
			(this->vel[i] +
			 2.f * this->v2[i] +
			 2.f * this->v3[i] +
			 this->v4[i]);

		vec3 net_acc = (this->aa[i] + acc) / 6.f;
		this->vel[i] += dt * net_acc;
	}

	glBindBuffer(GL_ARRAY_BUFFER, this->pvbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * pos.size(),
		&this->pos[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, this->svbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * size.size(),
		&this->size[0], GL_DYNAMIC_DRAW);

	check_gl_error("Loading data", 5007);
}

