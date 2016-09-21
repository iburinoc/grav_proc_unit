#pragma once

#include "gpu/glm.hpp"

struct Options {
	ivec2 dimensions;

	int particle_count;
	uint64_t seed;

	bool debug;
};

extern Options options;

void process_args(int argc, char **argv);

