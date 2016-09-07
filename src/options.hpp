#pragma once

#include "gpu/glm.hpp"

struct Options {
	ivec2 dimensions;
	bool debug;
};

extern Options options;

void process_args(int argc, char **argv);

