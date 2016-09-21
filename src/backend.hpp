#pragma once

#include <memory>

#include "buffer.hpp"

class Backend {
protected:
	// Use raw pointer so we can bind it later on
	StarBuffer *sb;
        int count;

	Backend(int count);

public:
	virtual ~Backend();

	void set_buffer(StarBuffer *sb);
	virtual void update(double dt) = 0;
};

class BasicBackend : public Backend {
	std::vector<vec4> pos;
	std::vector<vec3> vel;
	std::vector<vec3> acc;
public:
	BasicBackend(int count);
	virtual ~BasicBackend();

	virtual void update(double dt);
};

std::unique_ptr<Backend> get_backend_from_opts();

