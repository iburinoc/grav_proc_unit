#pragma once

#include <memory>

#include "gpu/gl.hpp"

#include "../particle_gen.hpp"

class Backend {
protected:
	GLuint pvbo, svbo;
        int count;
	std::unique_ptr<ParticleGen> p_gen;
	size_t stride;

	virtual void init();

	Backend(int count, std::unique_ptr<ParticleGen> p_gen, size_t stride);

public:
	virtual ~Backend();

	size_t pos_stride() const;
	int get_count() const;
	void set_vbos(GLuint pvbo, GLuint svbo);
	virtual void update(float dt) = 0;
};

