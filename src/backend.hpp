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
	virtual void update(float dt) = 0;
};

class BasicBackend : public Backend {
	std::vector<vec4> pos;
	std::vector<vec3> vel;
	std::vector<float> m;
	std::vector<bool> active;

	std::vector<vec4> x2;
	std::vector<vec4> x3;
	std::vector<vec4> x4;
	std::vector<vec3> v2;
	std::vector<vec3> v3;
	std::vector<vec3> v4;
	std::vector<vec3> aa;
public:
	BasicBackend(int count);
	virtual ~BasicBackend();

	virtual void update(float dt);
};

std::unique_ptr<Backend> get_backend_from_opts();

