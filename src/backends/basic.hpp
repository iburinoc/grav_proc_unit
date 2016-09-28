#pragma once

#include "base.hpp"

class BasicBackend : public Backend {
	std::vector<vec3> pos;
	std::vector<vec3> vel;
	std::vector<float> size;
	std::vector<float> m;
	std::vector<bool> active;

	std::vector<vec3> x2;
	std::vector<vec3> x3;
	std::vector<vec3> x4;
	std::vector<vec3> v2;
	std::vector<vec3> v3;
	std::vector<vec3> v4;
	std::vector<vec3> aa;
public:
	BasicBackend(int count);
	virtual ~BasicBackend();

	virtual void update(float dt);
};

